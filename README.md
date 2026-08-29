# CryptoDataCPP

**Status: archived historical demonstration; no live exchange support.**

CryptoDataCPP is a C++17 market-data experiment from 2021. It combines historical FTX REST and WebSocket clients with small static-data, streaming-market-data, and historical-market-data components. The repository is retained as evidence of older CMake, Conan, networking, and GoogleTest work; it is not a supported market-data library.

FTX is defunct. Its APIs, endpoints, authentication assumptions, and data contracts must be treated as obsolete. Do not connect this code to credentials, accounts, trading systems, or production infrastructure.

## Repository map

```text
cpp/src/feed/exchange/FTX/rest/  historical REST client
cpp/src/feed/exchange/FTX/ws/    historical WebSocket client
cpp/src/feed/exchange/FTX/util/  signing, HTTP, WebSocket, encoding, and time helpers
cpp/src/marketdata/              streaming and historical market-data abstractions
cpp/src/staticdata/              security metadata types
cpp/src/test/                    offline fixture-based GoogleTest executable
cpp/src/external/                vendored JSON header and websocketpp submodule
docs/Build.md                    legacy Windows build and test procedure
```

## What the tests cover

The checked-in suite defines thirteen deterministic, offline cases covering spot and futures metadata, duplicate validation, historical OHLCV parsing and date-filter query construction, malformed timestamps, ticker updates, subscription and authentication messages, JSON-null market-order construction, and an HMAC known vector. Tests never contact FTX and do not demonstrate that an external integration works.

## Build status

The preserved build uses the Conan 1.x CMake generator and was designed for Visual Studio. `conan.lock` fixes the direct and transitive dependency versions and Conan Center recipe revisions; Conan itself is also fixed. This improves repeatability but is not a cryptographically complete supply-chain guarantee because legacy sources, binaries, and hosted runners remain external. See [the build guide](docs/Build.md) for the historical path and its limitations.

Before configuring, initialize the required submodule:

```bash
git submodule update --init --recursive
```

CI targets Windows Server 2022 with Visual Studio 2022, Python 3.11, and Conan 1.66.0. It builds only offline tests, applies per-test and job timeouts, and does not enable the obsolete network examples. A successful run verifies source compatibility with that legacy toolchain only.

## Known limitations

- External FTX behavior is obsolete and intentionally excluded from verification.
- Dependency definitions use legacy Conan 1.x syntax and generators.
- The design predates current production requirements for secret management, schema evolution, reconnect/replay semantics, sequence integrity, observability, and supply-chain controls.
- Runtime DLL discovery on Windows depends on the Conan import layout documented in the build guide.
- The transport code now fails closed on certificate/hostname errors and bounds TCP/TLS/HTTP stream work after DNS resolution. Synchronous DNS resolution itself is not time-bounded, and the client remains archival and must not be connected to a replacement service.
- No release, compatibility, support, or operational-security commitment is made.

## License

[MIT](LICENSE), copyright 2021 Damian, covers the first-party project code. Vendored or submodule third-party code retains its own license notices.
