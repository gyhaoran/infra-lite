# infra-lite — Roadmap

**Timeframe: 8-12 weeks**  
**Last updated: 2026-04-15**

---

## Phase Overview

| Phase | Duration | Focus | Status |
|-------|----------|-------|--------|
| Phase 1: Project Foundation | 2-3 weeks | Documentation, positioning, error model | ✅ Complete |
| Phase 2: Unify Error Model | 2-3 weeks | infra::Error, Result<T>, parsing integration | ✅ Complete |
| Phase 3: Stabilize Parsing API | 3-4 weeks | API review, backward compatibility, documentation | ✅ Complete |
| Phase 4: Minimal Utilities | 2-3 weeks | Small utilities (span-like, string_view helpers) | ✅ Complete |
| Phase 5: Polish & Stabilize | 1-2 weeks | Performance, release prep, documentation | ✅ Complete |

---

## Phase 1: Project Foundation ✅ (Completed)

**Goal**: Establish project identity and development guidelines.

### Completed Tasks

- [x] Create project positioning document (`docs/positioning.md`)
- [x] Update README.md with one-liner and non-goals
- [x] Update README_CN.md (Chinese version)
- [x] Create CONTRIBUTING.md with development guidelines
- [x] Update features.md with non-goals and evolution strategy

### Key Outputs

- Clear project scope (what infra-lite IS and IS NOT)
- Development guidelines (SOLID, RAII, const-correctness)
- Commit conventions (small steps, local-only)
- Evolution strategy (5 allowed directions, forbidden directions)

---

## Phase 2: Unify Error Model ✅ (Completed)

**Goal**: Provide consistent error handling across all modules.

### Completed Tasks

- [x] Design `infra::Error` base type with Kind enum
- [x] Implement `Result<T>` monad with map/and_then
- [x] Create `Result<void>` specialization
- [x] Migrate `parsing/result.h` to use `infra::Error`
- [x] Update all primitives and combinators to use new API
- [x] Fix all examples to compile with new API
- [x] Add error_example demonstrating usage

### Key Outputs

- `include/infra/error.h`: Unified error handling
- `ParseResult<T>` now uses `infra::Error` internally
- Consistent error creation patterns: `ParseResult::success()`, `unexpected_char()`, `unexpected_end()`

### Metrics

- ✅ C++17 compatible
- ✅ Zero heap allocation
- ✅ All tests pass

---

## Phase 3: Stabilize Parsing API ✅ (Completed)

**Goal**: Ensure parsing API is stable and well-documented.

### Completed Tasks

- [x] API review: Identify any inconsistencies
- [x] Add more combinators (many, many1, choice, choice3, sequence2, then, skip, sep_by, between)
- [x] Add parser factory functions (char_p, char_if, one_of, none_of)
- [x] Document parser contract in code
- [x] Add string utilities (ltrim, rtrim, starts_with, ends_with, find_char, skip_word)
- [x] Update features.md with all combinators

### Deliverables

- ✅ Stable parsing API with all combinators
- ✅ Usage documentation for all combinators
- ✅ Parser contract well-documented in parser.h

---

## Phase 4: Minimal Utilities ✅ (Completed)

**Goal**: Add small, zero-dependency utilities that complement parsing.

### Completed Tasks

- [x] Span-like type (span<T> with iterator support)
- [x] String utilities (ltrim, rtrim, strcmp_view, etc.)
- [x] Optional logging (compile-time selectable via INFRA_LOG_ENABLED)
- [x] Common parser helpers (spaces, identifier, integer)

### Metrics

- ✅ Zero heap allocation in all utility modules
- ✅ C++17 compatible
- ✅ All tests pass

---

## Phase 5: Polish & Stabilize ✅ (Completed)

**Goal**: Finalize API and prepare for v1.0.

### Completed Tasks

- [x] API consistency review (combinators documented)
- [x] Add more tests (combinator, string_utils, error tests)
- [x] Performance benchmark
- [x] Release documentation (CHANGELOG.md)
- [x] Version header (version.h)

### Deliverables

- ✅ Performance benchmark (sub-microsecond operations)
- ✅ CHANGELOG.md following Keep a Changelog format
- ✅ version.h with semantic versioning
- Total: 6 test suites, all passing

---

## Decision Criteria for New Features

Before adding any new capability, evaluate against:

| Criterion | Requirement |
|-----------|-------------|
| **Size** | Can be understood in < 30 minutes |
| **Dependency** | Zero external dependencies |
| **Composition** | Works with existing modules |
| **Explicit** | No hidden state |
| **Scoped** | Solves a specific problem well |

### Forbidden Features

- ❌ Container libraries (vector, map alternatives)
- ❌ Large async/threading frameworks
- ❌ Reflection or metaprogramming
- ❌ GUI or UI components
- ❌ Network protocols
- ❌ Serialization frameworks

---

## Release Strategy

### Version Scheme

- **MAJOR**: Breaking changes to public API
- **MINOR**: New functionality (backward compatible)
- **PATCH**: Bug fixes (backward compatible)

### Current State

- **v0.1.0**: Initial release ready
- All phases complete
- API stable for v0.1.x

### Target: v1.0

- Stable parsing API
- Unambiguous error model
- Documented design principles
- All examples tested and working

---

## Resources

- [Positioning](positioning.md) — Project scope
- [Features](features.md) — Current capabilities
- [Contributing](../CONTRIBUTING.md) — Development guidelines

---

*This roadmap will be updated as the project evolves.*