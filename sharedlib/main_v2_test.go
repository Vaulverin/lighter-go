package main

import (
	"errors"
	"testing"
)

func TestSignInfoStatusValuesMatchAbi(t *testing.T) {
	tests := []struct {
		name string
		got  int
		want int
	}{
		{name: "OK", got: signInfoStatusOK, want: 0},
		{name: "TxInfoBufferTooSmall", got: signInfoStatusTxInfoBufferTooSmall, want: 1},
		{name: "ErrorBufferTooSmall", got: signInfoStatusErrorBufferTooSmall, want: 2},
		{name: "SigningFailed", got: signInfoStatusSigningFailed, want: 3},
		{name: "Panic", got: signInfoStatusPanic, want: 4},
	}

	for _, tt := range tests {
		if tt.got != tt.want {
			t.Fatalf("%s status = %d, want %d", tt.name, tt.got, tt.want)
		}
	}
}

func TestWriteStringToBytesBufferReportsRequiredLength(t *testing.T) {
	dst := make([]byte, 2)

	status, length := writeStringToBytesBuffer(dst, "abcd")

	if status != signInfoStatusTxInfoBufferTooSmall {
		t.Fatalf("status = %v, want %v", status, signInfoStatusTxInfoBufferTooSmall)
	}
	if length != 4 {
		t.Fatalf("length = %d, want 4", length)
	}
}

func TestWriteStringToBytesBufferCopiesWithoutTerminatorRequirement(t *testing.T) {
	dst := make([]byte, 4)

	status, length := writeStringToBytesBuffer(dst, "abcd")

	if status != signInfoStatusOK {
		t.Fatalf("status = %v, want %v", status, signInfoStatusOK)
	}
	if length != 4 {
		t.Fatalf("length = %d, want 4", length)
	}
	if got := string(dst[:length]); got != "abcd" {
		t.Fatalf("buffer = %q, want %q", got, "abcd")
	}
}

func TestWriteSignInfoResultReportsSmallTxInfoBuffer(t *testing.T) {
	txInfoBuffer := make([]byte, 8)
	errorBuffer := make([]byte, 1024)

	result := writeSignInfoResult(`{"AccountIndex":65,"Sig":"abc"}`, nil, txInfoBuffer, errorBuffer)

	if result.Status != signInfoStatusTxInfoBufferTooSmall {
		t.Fatalf("status = %v, want %v", result.Status, signInfoStatusTxInfoBufferTooSmall)
	}
	if result.TxInfoLength <= len(txInfoBuffer) {
		t.Fatalf("tx info length = %d, want > %d", result.TxInfoLength, len(txInfoBuffer))
	}
	if result.ErrorLength != 0 {
		t.Fatalf("error length = %d, want 0", result.ErrorLength)
	}
}

func TestWriteSignInfoResultWritesSigningErrorText(t *testing.T) {
	txInfoBuffer := make([]byte, 1024)
	errorBuffer := make([]byte, 1024)

	result := writeSignInfoResult("", errors.New("signing failed"), txInfoBuffer, errorBuffer)

	if result.Status != signInfoStatusSigningFailed {
		t.Fatalf("status = %v, want %v", result.Status, signInfoStatusSigningFailed)
	}
	if result.TxInfoLength != 0 {
		t.Fatalf("tx info length = %d, want 0", result.TxInfoLength)
	}
	if result.ErrorLength != len("signing failed") {
		t.Fatalf("error length = %d, want %d", result.ErrorLength, len("signing failed"))
	}
	if got := string(errorBuffer[:result.ErrorLength]); got != "signing failed" {
		t.Fatalf("error buffer = %q, want %q", got, "signing failed")
	}
}

func TestWriteSignInfoResultReportsSmallErrorBuffer(t *testing.T) {
	txInfoBuffer := make([]byte, 1024)
	errorBuffer := make([]byte, 4)
	for i := range errorBuffer {
		errorBuffer[i] = 'x'
	}
	originalErrorBuffer := append([]byte(nil), errorBuffer...)
	errText := "signing failed with long diagnostic"

	result := writeSignInfoResult("", errors.New(errText), txInfoBuffer, errorBuffer)

	if result.Status != signInfoStatusErrorBufferTooSmall {
		t.Fatalf("status = %v, want %v", result.Status, signInfoStatusErrorBufferTooSmall)
	}
	if result.TxInfoLength != 0 {
		t.Fatalf("tx info length = %d, want 0", result.TxInfoLength)
	}
	if result.ErrorLength != len(errText) {
		t.Fatalf("error length = %d, want %d", result.ErrorLength, len(errText))
	}
	if string(errorBuffer) != string(originalErrorBuffer) {
		t.Fatalf("error buffer = %q, want unchanged %q", string(errorBuffer), string(originalErrorBuffer))
	}
}
