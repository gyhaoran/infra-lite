# infra-lite

**A lightweight, embeddable C/C++ infrastructure library.**

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)

---

## What is infra-lite?

infra-lite is a collection of **small, reusable infrastructure components** that every project needs but are too small for frameworks like Abseil or Folly.

> "The foundation before the convenience."

**Core focus**: Parsing primitives and error handling utilities that are explicit, composable, and zero-dependency.

---

## Philosophy

| Principle | Meaning |
|-----------|---------|
| **Infrastructure over features** | Foundation before convenience |
| **Simple over complete** | Do fewer things, do them well |
| **Explicit over magic** | No hidden state, no surprises |
| **Composable over monolithic** | Small pieces that fit together |

---

## Key Characteristics

- **Header-first** — No compilation overhead, just include and use
- **Zero dependencies** — Pure C++17, no external libraries
- **Embeddable** — Fits into constrained environments (embedded, EDA)
- **Explicit** — No hidden state, no magic
- **Composable** — Small primitives that combine cleanly

---

## Non-goals (What infra-lite is NOT)

infra-lite is intentionally constrained. It is NOT:

1. ❌ A container library (no STL alternatives)
2. ❌ An async framework (no large-scale concurrency)
3. ❌ A reflection system
4. ❌ A "kitchen-sink" utility library
5. ❌ Exception-based (errors via explicit return types)
6. ❌ STL-dependent for core abstractions

For larger needs, consider [Abseil](https://abseil.io/), [Folly](https://github.com/facebook/folly), or [Boost](https://www.boost.org/).

---

## Current Capabilities

### Parsing Infrastructure
- Unified result/error model
- Primitive parsers (char matching, predicates)
- Parser combinators (left/right associative chaining)
- Explicit whitespace handling

### Design Principles
- No implicit allocation
- No hidden state
- Control flow is predictable and traceable

See [docs/features.md](docs/features.md) for complete capability list.

---

## Quick Start

```cpp
#include "infra/parsing/parser.h"
#include "infra/parsing/combinators.h"

using namespace infra::parsing;

// Simple parser: match one or more digits
auto number = many1(char_p(isdigit));

// Use it
const char* input = "123abc";
auto result = number(input);
if (result) {
    // result.value() contains "123"
    // result.end() points to "abc"
}
```

---

## Build & Test

```bash
# Clone and build
mkdir build && cd build
cmake ..
cmake --build .

# Run tests
ctest

# Run examples
./examples/calculator
./examples/cmd_parser
```

---

## Project Structure

```
include/
├── infra/
│   ├── error.h          # Unified error model (infra::Error, Result<T>)
│   ├── logging.h       # Compile-time logging (INFRA_LOG_ENABLED)
│   ├── parsing/
│   │   ├── parser.h     # Parser contract
│   │   ├── primitives.h # char_p, char_if, one_of, none_of
│   │   ├── combinators.h# many, choice, sequence, skip, sep_by, between
│   │   ├── helpers.h   # spaces, identifier, integer
│   │   ├── chain.h     # Operator chaining
│   │   └── result.h    # Parse result (uses infra::Error)
│   └── util/
│       ├── span.h      # Non-owning memory view
│       ├── string_utils.h# ltrim, strcmp_view, etc.
│       └── char_stream.h# Input utilities
docs/
├── features.md          # Current capabilities
├── positioning.md        # Project positioning
├── roadmap.md           # Development plan
└── parsing_framework_guide.md
examples/
├── calculator.cpp       # Expression evaluator
├── cmd_parser.cpp       # CLI argument parser
├── mini_config/         # Config file parser
└── error_example/       # Error handling examples
tests/
├── test_combinators.cpp
├── test_string_utils.cpp
├── test_error.cpp
├── test_span.cpp
└── test_logging.cpp
```

---

## Documentation

- [Positioning](docs/positioning.md) — Project scope and non-goals
- [Features](docs/features.md) — Supported capabilities
- [Roadmap](docs/roadmap.md) — Development plan (8-12 weeks)
- [Parsing Guide](docs/parsing_framework_guide.md) — Design rationale

---

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md) for:
- Code style guidelines
- Commit conventions
- Design principles (SOLID, RAII, const-correctness)

---

## License

MIT License. See [LICENSE](LICENSE).