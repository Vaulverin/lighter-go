#ifndef LIGHTER_SIGN_INFO_V2_H
#define LIGHTER_SIGN_INFO_V2_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
#define LIGHTER_SIGN_INFO_V2_EXPORT __declspec(dllexport)
#else
#define LIGHTER_SIGN_INFO_V2_EXPORT
#endif

LIGHTER_SIGN_INFO_V2_EXPORT int SignCreateOrderInfoV2(int cMarketIndex, long long cClientOrderIndex, long long cBaseAmount,
                                                      int cPrice, int cIsAsk, int cOrderType, int cTimeInForce,
                                                      int cReduceOnly, int cTriggerPrice, long long cOrderExpiry,
                                                      uint8_t cSkipNonce, long long cNonce, int cApiKeyIndex,
                                                      long long cAccountIndex, char* txInfoBuffer, int txInfoCapacity,
                                                      int* txInfoLength, char* errorBuffer, int errorCapacity,
                                                      int* errorLength);

LIGHTER_SIGN_INFO_V2_EXPORT int SignCancelOrderInfoV2(int cMarketIndex, long long cOrderIndex, uint8_t cSkipNonce,
                                                      long long cNonce, int cApiKeyIndex, long long cAccountIndex,
                                                      char* txInfoBuffer, int txInfoCapacity, int* txInfoLength,
                                                      char* errorBuffer, int errorCapacity, int* errorLength);

LIGHTER_SIGN_INFO_V2_EXPORT int SignModifyOrderInfoV2(int cMarketIndex, long long cIndex, long long cBaseAmount,
                                                      long long cPrice, long long cTriggerPrice, uint8_t cSkipNonce,
                                                      long long cNonce, int cApiKeyIndex, long long cAccountIndex,
                                                      char* txInfoBuffer, int txInfoCapacity, int* txInfoLength,
                                                      char* errorBuffer, int errorCapacity, int* errorLength);

LIGHTER_SIGN_INFO_V2_EXPORT int SignCancelAllOrdersInfoV2(int cTimeInForce, long long cTime, uint8_t cSkipNonce,
                                                          long long cNonce, int cApiKeyIndex, long long cAccountIndex,
                                                          char* txInfoBuffer, int txInfoCapacity, int* txInfoLength,
                                                          char* errorBuffer, int errorCapacity, int* errorLength);

int SignCreateOrderInfoV2Go(int cMarketIndex, long long cClientOrderIndex, long long cBaseAmount,
                            int cPrice, int cIsAsk, int cOrderType, int cTimeInForce,
                            int cReduceOnly, int cTriggerPrice, long long cOrderExpiry,
                            uint8_t cSkipNonce, long long cNonce, int cApiKeyIndex,
                            long long cAccountIndex, char* txInfoBuffer, int txInfoCapacity,
                            int* txInfoLength, char* errorBuffer, int errorCapacity,
                            int* errorLength);

int SignCancelOrderInfoV2Go(int cMarketIndex, long long cOrderIndex, uint8_t cSkipNonce,
                            long long cNonce, int cApiKeyIndex, long long cAccountIndex,
                            char* txInfoBuffer, int txInfoCapacity, int* txInfoLength,
                            char* errorBuffer, int errorCapacity, int* errorLength);

int SignModifyOrderInfoV2Go(int cMarketIndex, long long cIndex, long long cBaseAmount,
                            long long cPrice, long long cTriggerPrice, uint8_t cSkipNonce,
                            long long cNonce, int cApiKeyIndex, long long cAccountIndex,
                            char* txInfoBuffer, int txInfoCapacity, int* txInfoLength,
                            char* errorBuffer, int errorCapacity, int* errorLength);

int SignCancelAllOrdersInfoV2Go(int cTimeInForce, long long cTime, uint8_t cSkipNonce,
                                long long cNonce, int cApiKeyIndex, long long cAccountIndex,
                                char* txInfoBuffer, int txInfoCapacity, int* txInfoLength,
                                char* errorBuffer, int errorCapacity, int* errorLength);

#ifdef __cplusplus
}
#endif

#endif
