# infra-lite v1.0.0 Release Notes

**Release Date**: 2026-04-15  
**Type**: Major Release (API Stable)

---

## Announcement

**infra-lite v1.0.0 is now available!**

This release marks the library's API as stable. From this point forward, breaking changes will only occur in major version bumps.

---

## What is infra-lite?

infra-lite is a **lightweight, embeddable C++ infrastructure library** providing:

- **Parser combinators** - Build parsers from small, composable pieces
- **Error handling** - Explicit Result type with zero overhead
- **Memory utilities** - Zero-allocation span type
- **String utilities** - Fast string operations without allocation
- **Compile-time logging** - Zero-cost logging when disabled

---

## Quick Start

```cpp
#include "infra/parsing/parser.h"
#include "infra/parsing/combinators.h"
#include "infra/parsing/helpers.h"

using namespace infra::parsing;

// Parse an integer
auto result = integer("123abc");
if (result) {
    printf("Value: %d\n", result.value());
}

// Parse comma-separated numbers
ParseResult<char> parse_digit(const char* s) {
    return satisfy(s, ::isdigit);
}
auto nums = sep_by("1,2,3", parse_digit, char_p(','));
```

---

## What's New in v1.0.0

### Core Modules (Stable)

| Module | Description | Status |
|--------|-------------|--------|
| `infra::Error` | Unified error type | ✅ Stable |
| `infra::Result<T>` | Result monad | ✅ Stable |
| `infra::parsing::Parser<T>` | Parser contract | ✅ Stable |
| `infra::util::span<T>` | Non-owning memory view | ✅ Stable |
| `infra::logging` | Compile-time logging | ✅ Stable |

### Parser Combinators

```cpp
// Repetition
many(s, parser)     // Zero or more
many1(s, parser)    // One or more

// Choice
choice(s, p1, p2)   // First success
choice3(s, p1, p2, p3)  // Try three

// Sequence
sequence2(s, p1, p2)  // Run in sequence

// Utility
optional(s, p)      // Default on failure
skip(s, p)          // Discard result
sep_by(s, item, sep)  // Separated list
between(s, open, content, close)  // Surrounded
```

### Error Handling

```cpp
#include "infra/error.h"

infra::Result<int> divide(int a, int b) {
    if (b == 0) return make_error("division by zero");
    return a / b;
}

auto r = divide(10, 2);
if (r) {
    printf("Result: %d\n", *r);
} else {
    printf("Error: %s\n", r.error().message);
}
```

---

## Performance

All operations are **sub-microsecond** with **zero heap allocation**:

| Operation | Performance |
|-----------|-------------|
| char_p | ~0.01 us/op |
| many/many1 | ~0.7 us/op |
| choice/sequence | ~0.02 us/op |
| ltrim/starts_with | ~0.01 us/op |
| span iteration | ~0.02 us/op |

---

## Supported Environments

- **C++ Standard**: C++17 or later
- **Platforms**: Windows, Linux, macOS
- **Compiler**: GCC, Clang, MSVC
- **Dependencies**: Zero (header-only)

---

## Migration Guide

### From v0.x to v1.0.0

**No breaking changes** - v1.0.0 is fully backward compatible with v0.1.x.

---

## Examples

The repository includes working examples:

| Example | Description |
|---------|-------------|
| `calculator` | Expression evaluator |
| `command_parser` | CLI argument parser |
| `mini_config` | Config file parser |
| `error_example` | Error handling demo |
| `benchmark` | Performance benchmarks |

---

## Documentation

- [README.md](README.md) - Quick start and overview
- [docs/features.md](docs/features.md) - Complete feature list
- [docs/positioning.md](docs/positioning.md) - Project scope
- [docs/roadmap.md](docs/roadmap.md) - Development plan
- [docs/parsing_framework_guide.md](docs/parsing_framework_guide.md) - Parsing tutorial
- [CONTRIBUTING.md](CONTRIBUTING.md) - Development guidelines

---

## Acknowledgments

Thanks to all contributors and users of infra-lite.

---

## Links

- **Repository**: https://github.com/gyhaoran/infra-lite
- **Issues**: https://github.com/gyhaoran/infra-lite/issues
- **License**: MIT

---

*Built with the philosophy: "Small, explicit, composable — not big, magic, monolithic."*
