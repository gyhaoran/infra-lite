# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [1.0.0] - 2026-04-15

### Changed

- **Breaking**: Version bump to 1.0.0 - API now considered stable

### Added

#### Error Model

### Added

#### Error Model
- `infra::Error` - Unified error type with Kind enum
- `infra::Result<T>` - Result monad with map/and_then operations
- Consistent error creation: `success()`, `unexpected_char()`, `unexpected_end()`

#### Parsing Infrastructure
- `infra::parsing::Parser<T>` - Parser contract
- Primitive parsers:
  - `char_p(c)` - Match exact character
  - `char_if(pred)` - Match if predicate returns true
  - `one_of(chars)` - Match any char in string
  - `none_of(chars)` - Match any char NOT in string
  - `any_char()` - Consume any single character
  - `satisfy(s, pred)` - Consume if predicate matches
- Parser combinators:
  - `optional(s, p)` - Return default on failure
  - `lookahead(s, p)` - Match without consuming input
  - `skip(s, p)` - Discard result, return position
  - `many(s, p)` - Match zero or more
  - `many1(s, p)` - Match one or more
  - `choice(s, p1, p2)` - Return first success
  - `sequence2(s, p1, p2)` - Run two parsers in sequence
  - `sep_by(s, item, sep)` - Parse separated list
  - `between(s, open, content, close)` - Parse surrounded content
  - `then(s, p, f)` - Transform result with function

#### Utilities
- `infra::span<T>` - Non-owning memory view
  - `data()`, `size()`, `begin()`, `end()`
  - `subspan()`, `first()`, `last()`
- `infra::util::ltrim(s)` - Skip leading whitespace
- `infra::util::rtrim(s)` - Remove trailing whitespace
- `infra::util::starts_with(s, prefix)` - Prefix check
- `infra::util::ends_with(s, suffix)` - Suffix check
- `infra::util::find_char(s, c)` - Find character
- `infra::util::strcmp_view(a, b)` - String comparison
- `infra::util::skip_word(s)` - Skip word

#### Logging
- Compile-time logging with `INFRA_LOG_ENABLED`
- Zero overhead when disabled
- `INFRA_LOG_INFO()`, `INFRA_LOG_WARN()`, `INFRA_LOG_ERROR()`

#### Examples
- Calculator - Expression evaluator
- Command parser - CLI argument parser
- Mini config - Config file parser
- Error example - Error handling demonstration
- Benchmark - Performance benchmarks

#### Testing
- Unit tests for all modules
- 6 test suites with 100% pass rate

[Unreleased]: https://github.com/gyhaoran/infra-lite/compare/v1.0.0...HEAD
[1.0.0]: https://github.com/gyhaoran/infra-lite/releases/tag/v1.0.0
