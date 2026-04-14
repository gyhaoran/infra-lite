# infra-lite — Project Positioning

## One-liner

**A lightweight, embeddable C/C++ infrastructure library providing composable, explicit, zero-dependency parsing primitives and error handling utilities.**

---

## Problem Statement

infra-lite solves the "repeatable infrastructure problem": the countless small utilities that every project needs but are too small for a framework like Abseil or Folly, yet too important to leave copy-pasted across codebases.

---

## Use Cases

- **CLI tools** — parsing command-line arguments, config files
- **EDA/embedded** — constrained environments requiring minimal dependencies
- **Language tooling** — parsers for small DSLs, expression evaluators
- **Library foundation** — building blocks for more complex systems

---

## Non-goals (What infra-lite is NOT)

1. **NOT a container library** — Does not duplicate STL or Abseil containers
2. **NOT an async framework** — No large-scale concurrency primitives (unlike Folly)
3. **NOT a reflection system** — No runtime type introspection
4. **NOT a "kitchen-sink" utility library** — No feature creep or bloat
5. **NOT a full parsing framework** — Only low-level composable primitives
6. **NOT exception-based** — Error handling via explicit return types
7. **NOT STL-dependent for core abstractions** — Minimal standard library usage
8. **NOT a replacement for Abseil/Folly** — These serve different scales

---

## Design Philosophy

> "Small, explicit, composable — not big, magic, monolithic."

- **Infrastructure over features** — Foundation before convenience
- **Simple over complete** — Do fewer things, do them well
- **Explicit over implicit** — No hidden state, no magic
- **Composable over monolithic** — Small pieces that fit together

---

## Comparison with Similar Projects

| Library | infra-lite Difference |
|---------|---------------------|
| **Abseil** | Abseil is Google's general-purpose foundation; infra-lite is intentionally smaller and more constrained |
| **Folly** | Folly provides large-scale async; infra-lite has zero async dependencies |
| **Boost** | Boost is comprehensive and heavy; infra-lite is minimal and embeddable |

---

## Evolution Strategy

### Allowed Directions (Max 5)

1. **Unified error model** — Consistent error handling across modules
2. **Parsing primitives** — Deeper combinator support
3. **Small utilities** — string_view-like helpers, no allocation
4. **Optional logging** — Minimal, compile-time selectable
5. **Explicit memory utilities** — unique_ptr, span-like types

### Forbidden Directions

- Container libraries (vector, map alternatives)
- Large async/threading frameworks
- Reflection or metaprogramming systems
- GUI or UI components
- Network protocols
- Serialization frameworks

---

## Target Scale

| Metric | Target |
|--------|--------|
| Header files | < 20 |
| Total LOC (headers only) | < 5000 |
| Dependencies | Zero (no external deps) |
| C++ Standard | C++17 minimum |
| Build time impact | < 1 second |

---

*Last updated: 2026-04-14*