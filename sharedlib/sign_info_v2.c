#include "sign_info_v2.h"

#include "signal_stack.h"

LIGHTER_SIGN_INFO_V2_EXPORT int SignCreateOrderInfoV2(int cMarketIndex, long long cClientOrderIndex, long long cBaseAmount,
                                                      int cPrice, int cIsAsk, int cOrderType, int cTimeInForce,
                                                      int cReduceOnly, int cTriggerPrice, long long cOrderExpiry,
                                                      uint8_t cSkipNonce, long long cNonce, int cApiKeyIndex,
                                                      long long cAccountIndex, char* txInfoBuffer, int txInfoCapacity,
                                                      int* txInfoLength, char* errorBuffer, int errorCapacity,
                                                      int* errorLength) {
    lighter_ensure_large_sigaltstack();
    return SignCreateOrderInfoV2Go(cMarketIndex, cClientOrderIndex, cBaseAmount,
                                   cPrice, cIsAsk, cOrderType, cTimeInForce,
                                   cReduceOnly, cTriggerPrice, cOrderExpiry,
                                   cSkipNonce, cNonce, cApiKeyIndex, cAccountIndex,
                                   txInfoBuffer, txInfoCapacity, txInfoLength,
                                   errorBuffer, errorCapacity, errorLength);
}

LIGHTER_SIGN_INFO_V2_EXPORT int SignCancelOrderInfoV2(int cMarketIndex, long long cOrderIndex, uint8_t cSkipNonce,
                                                      long long cNonce, int cApiKeyIndex, long long cAccountIndex,
                                                      char* txInfoBuffer, int txInfoCapacity, int* txInfoLength,
                                                      char* errorBuffer, int errorCapacity, int* errorLength) {
    lighter_ensure_large_sigaltstack();
    return SignCancelOrderInfoV2Go(cMarketIndex, cOrderIndex, cSkipNonce, cNonce,
                                   cApiKeyIndex, cAccountIndex, txInfoBuffer,
                                   txInfoCapacity, txInfoLength, errorBuffer,
                                   errorCapacity, errorLength);
}

LIGHTER_SIGN_INFO_V2_EXPORT int SignModifyOrderInfoV2(int cMarketIndex, long long cIndex, long long cBaseAmount,
                                                      long long cPrice, long long cTriggerPrice, uint8_t cSkipNonce,
                                                      long long cNonce, int cApiKeyIndex, long long cAccountIndex,
                                                      char* txInfoBuffer, int txInfoCapacity, int* txInfoLength,
                                                      char* errorBuffer, int errorCapacity, int* errorLength) {
    lighter_ensure_large_sigaltstack();
    return SignModifyOrderInfoV2Go(cMarketIndex, cIndex, cBaseAmount, cPrice,
                                   cTriggerPrice, cSkipNonce, cNonce, cApiKeyIndex,
                                   cAccountIndex, txInfoBuffer, txInfoCapacity,
                                   txInfoLength, errorBuffer, errorCapacity,
                                   errorLength);
}

LIGHTER_SIGN_INFO_V2_EXPORT int SignCancelAllOrdersInfoV2(int cTimeInForce, long long cTime, uint8_t cSkipNonce,
                                                          long long cNonce, int cApiKeyIndex, long long cAccountIndex,
                                                          char* txInfoBuffer, int txInfoCapacity, int* txInfoLength,
                                                          char* errorBuffer, int errorCapacity, int* errorLength) {
    lighter_ensure_large_sigaltstack();
    return SignCancelAllOrdersInfoV2Go(cTimeInForce, cTime, cSkipNonce, cNonce,
                                       cApiKeyIndex, cAccountIndex, txInfoBuffer,
                                       txInfoCapacity, txInfoLength, errorBuffer,
                                       errorCapacity, errorLength);
}
