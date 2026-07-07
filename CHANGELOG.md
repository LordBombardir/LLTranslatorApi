# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [26.20.0] - 2026-07-07

### Added
- Upgraded the project to support LeviLamina 26.20.0.

### Fixed
- Fixed compilation and linker conflicts on `AvailableCommandsPacket` constructors.
- Fixed a crash on mod reload/disable caused by a dangling background coroutine loop in `CacheCleanerTask`.
- Fixed potential pointer-reuse collisions in the packet translator cache by clearing the cache immediately when transmission finishes.
- Replaced unsafe `const_cast` modifications of outbound packets with secure copy-cloning.

### Changed
- Re-implemented string placeholder replacements using a single-pass linear scan instead of recursive Boyer-Moore replaces.
- Added thread-safe mutex guards to the persistent placeholders map.
- Optimized string hash formatting using C++20 `std::format`.
- Removed the dead `Utils` class and cleaned up its imports.
