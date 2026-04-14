# infra-lite — Roadmap

**Timeframe: 8-12 weeks**  
**Last updated: 2026-04-14**

---

## Phase Overview

| Phase | Duration | Focus | Status |
|-------|----------|-------|--------|
| Phase 1: Project Foundation | 2-3 weeks | Documentation, positioning, error model | ✅ Complete |
| Phase 2: Unify Error Model | 2-3 weeks | infra::Error, Result<T>, parsing integration | ✅ Complete |
| Phase 3: Stabilize Parsing API | 3-4 weeks | API review, backward compatibility, documentation | 🔄 Current |
| Phase 4: Minimal Utilities | 2-3 weeks | Small utilities (span-like, string_view helpers) | 📋 Planned |

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

## Phase 3: Stabilize Parsing API 🔄 (Current)

**Goal**: Ensure parsing API is stable and well-documented.

### Current Tasks

- [ ] API review: Identify any inconsistencies
- [ ] Add more combinators (many, choice, sequence)
- [ ] Document parser contract in code
- [ ] Add usage examples for each combinator
- [ ] Performance benchmarking (optional)

### Why Now?

The error model refactor provides an opportunity to:
1. Review API design while changes are fresh
2. Ensure backward compatibility before external adoption
3. Document lessons learned

### Deliverables

- Stable parsing API with backward compatibility guarantees
- Usage documentation for all combinators
- Performance characteristics documented

---

## Phase 4: Minimal Utilities (Planned)

**Goal**: Add small, zero-dependency utilities that complement parsing.

### Candidate Features

1. **Span-like type** (contiguous memory view without ownership)
   - Zero allocation
   - Bounds checking (debug mode)
   - Compatible with existing APIs

2. **String view helpers**
   - Trim, split, strip operations
   - Zero allocation
   - Constexpr where possible

3. **Optional logging** (compile-time selectable)
   - Minimal footprint
   - No runtime overhead when disabled
   - Simple API

### Constraints

- Must not duplicate STL
- Must be small and composable
- Must have zero external dependencies
- Must be header-only

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

- **v0.1.x**: Initial development phase
- API subject to change
- No backward compatibility guarantees yet

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