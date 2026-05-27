# lighter-go

This repository serves as the reference implementation of signing & hashing of Lighter transactions.
The sharedlib is compiled for a variety of platforms.
- macOS (darwin) dynamic library (.dylib) for arm architecture (M processor, not Intel)
- linux shared object (.so) for both amd64 and arm architectures
- windows .dll (dynamic-link library) for amd64 architecture

The go SDK implements just the core signing, as well as a small HTTP client so that users can:
- not specify the nonce of the transaction (this will result in an HTTP call, so beware)
- check that a client was initialized correctly, by verifying that the given API key matches the one on the server 

The [Python SDK](https://github.com/elliottech/lighter-python) offers support for HTTP and WebSocket functionality as well as [examples](https://github.com/elliottech/lighter-python/tree/main/examples) on how to generate the API keys, how to create and cancel orders, generate AUTH tokens for various HTTP/WS endpoints which require them.       

All generated shared libraries follow the naming convention `lighter_signer_{os}_{arch}` where os is linux/windows/darwin and arch is amd64(x86) or arm64.\
The build & accompanying `.h` files can be found in the release notes [here](https://github.com/elliottech/lighter-go/releases).\
If you'd like to compile your own binaries, the commands are in the `justfile`.

## .NET hot-path signer fork

This fork keeps the upstream signer API intact and adds a small V2 ABI for latency-sensitive .NET order flows. The
change was made for an in-process trading service that signs many Lighter create/cancel/modify/cancel-all order
transactions from CoreCLR through P/Invoke.

The V2 ABI is intentionally limited to hot order signing:

- `SignCreateOrderInfoV2`
- `SignCancelOrderInfoV2`
- `SignModifyOrderInfoV2`
- `SignCancelAllOrdersInfoV2`

These public C wrapper exports call internal Go workers after installing a larger Linux alternate signal stack on the
calling thread. This reduces the risk of CoreCLR/Go signal-stack interaction failures when the Go runtime is embedded
inside a .NET process.

The V2 functions return only the `tx_info` JSON needed by the exchange API, plus an error string when signing fails.
They write into caller-owned buffers instead of returning C heap strings. This avoids per-order `C.CString` allocations
for unused fields such as `tx_hash` and `message_to_sign`, removes `Free` ownership from the hot path, and lets the
caller resize exactly once when a buffer is too small.

Create and modify V2 order signing use the non-integrator transaction option path because the .NET adapter currently
passes zero integrator values. If integrator signing is needed later, add a separate V2 integrator entry point rather
than adding default hot-path work back to these functions.

The existing ABI remains available for non-hot operations and for compatibility with upstream callers.


## Transactions
```
=== Client ===
CreateClient
CheckClient

=== API Key ===
CreateAuthToken
SignChangePubKey
GenerateAPIKey

=== Order ===
SignCreateOrder
SignCreateGroupedOrders
SignCancelOrder
SignCancelAllOrders
SignModifyOrder

=== Leverage & Margin ===
SignUpdateLeverage
SignUpdateMargin

=== Transfers ===
SignWithdraw
SignTransfer

=== Sub account & pools ===
SignCreateSubAccount
SignCreatePublicPool
SignUpdatePublicPool
SignMintShares
SignBurnShares

=== Staking ===
SignStakeAssets
SignUnstakeAssets

=== Integrators ===
SignApproveIntegrator
```

## How to specify an account
Accounts are loaded into the signer by calling the `CreateClient` method. If you wish to load multiple API keys in the signer, you need to call the method multiple times, each time with the correct private key.

By default, signer will work out of the box with 1 client and no need to manage nonces in any specific way. Just pass `-1, 255, 0` for all methods (more explanations below).

You can call `CheckClient` to verify that the provided Private key & (apiKeyIndex, accountIndex) are configured correctly. 
This checks that the public key associated with the pair (apiKey,account) matches the one from the exchange.

The majority of methods receive 3 arguments at the end:
- `nonce`
  - default `-1` 
  - required to be strictly incremental
  - you can fetch the next nonce using `nextNonce` HTTP call
  - if default is passed, signer will do the HTTP call automatically
  - ideally the caller should manage nonces locally to avoid latency
- `apiKeyIndex`
  - default `255` 
  - specified which API key will be used for the specific transaction
  - all API keys are equal. Orders created by one API key can be canceled by others, for example
  - each API key has its own nonce
  - if default is passed, signer will use the default txClient
- `accountIndex`
  - default `0`
  - specified which account will be used for the specific transaction
  - this can be a subaccount or a different main account all together 
  - if default is passed, signer will use the default txClient

**Note:** in order to use the default client, you need to bash both the default values for `apiKeyIndex` and `accountIndex`

## Auth tokens

Auth tokens are used to call various HTTP & WS endpoints which hold sensitive information, like open orders.
An auth token is valid for 8 hours.

`CreateAuthToken(deadline=0)` will result in a token that's valid for 7 hours from now.

Calling `CreateAuthToken` with an expiry 20 hours in the future will work, but the token will start to be valid in 12 hours, because the max accepted deadline duration by the server is 8 hours. \
This still allows you to generate all the tokens ahead of time and use them accordingly. \
Such an approach (both implementation & how to manage them) can be found in great details in the [python-sdk](https://github.com/elliottech/lighter-python/tree/main/examples/read-only-auth).

**Note:** auth tokens are bound to an API key. Changing the API key to something else **will invalidate** all generated auth tokens.  
