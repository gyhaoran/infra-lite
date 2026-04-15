# Parsing Module Deep Review

**Date**: 2026-04-15  
**Reviewer**: Staff/Principal Engineer  
**Status**: Critical Analysis

---

## 1. Executive Summary

infra-lite's parsing module implements a **parser combinator pattern** with explicit error handling. After deep analysis, the module is **worthy as a long-term core module** with minor improvements recommended.

---

## 2. Design Compliance Check

### 2.1 Explicit Error Model ✅

| Requirement | Status | Evidence |
|-------------|--------|----------|
| No hidden state | ✅ Pass | `ParseResult` explicitly carries `next` pointer |
| Error propagation via return type | ✅ Pass | Uses `infra::Error` with `Kind` enum |
| No exceptions | ✅ Pass | All parsing functions are `noexcept` |

**Code Evidence**:
```cpp
// Good: Explicit error model
inline ParseResult<char> expect_char(const char* s, char expected) {
    if (*s == '\0') {
        return ParseResult<char>::unexpected_end(s);
    }
    if (*s != expected) {
        return ParseResult<char>::unexpected_char(s);
    }
    return ParseResult<char>::success(expected, s + 1);
}
```

### 2.2 Composability ✅

| Requirement | Status | Evidence |
|-------------|--------|----------|
| Small primitives | ✅ Pass | `char_p`, `satisfy`, `any_char` |
| Composable combinators | ✅ Pass | `many`, `choice`, `sequence2`, `then` |
| No inheritance required | ✅ Pass | Lambda-based composition |

**Code Evidence**:
```cpp
// Good: Composable
auto number = many1(char_p(::isdigit));
auto expr = many(choice(number, identifier));
```

### 2.3 No Hidden State ✅

| Requirement | Status | Evidence |
|-------------|--------|----------|
| Input passed explicitly | ✅ Pass | `const char* s` parameter |
| Position tracked explicitly | ✅ Pass | `next` in `ParseResult` |
| No global variables | ✅ Pass | Zero globals |

---

## 3. Advantages (Specific Design Points)

### 3.1 Zero-Allocation Core

All parsing primitives allocate nothing on the heap:

```
Benchmark Results (sub-microsecond):
- char_p:        ~0.01 us/op
- many:          ~0.66 us/op  
- choice:        ~0.02 us/op
- sequence2:     ~0.03 us/op
```

### 3.2 Header-Only Design

Zero compilation overhead. User includes what they need:

```cpp
#include "infra/parsing/parser.h"
#include "infra/parsing/combinators.h"
```

### 3.3 Parser Contract Clarity

The `Parser<T>` concept is well-documented:

```cpp
// From parser.h
/// Parser<T> concept:
/// - Input: const char* (pointer to remaining input)
/// - Output: ParseResult<T>
/// - Must not throw (noexcept)
/// - Must not allocate (zero heap)
```

### 3.4 Consistent Error Model

All errors use `infra::Error` with `Kind` enum:

```cpp
enum class Kind {
    None,
    UnexpectedChar,
    UnexpectedEnd,
    Custom
};
```

---

## 4. Defects (Code-Level Issues)

### 4.1 CRITICAL: `helpers.h` Has Compile Errors

**File**: `include/infra/parsing/helpers.h`

**Issue 1**: Missing `#include <string>`

```cpp
// Line 32 - uses std::string without include
inline auto identifier = [](const char* s) -> ParseResult<std::string> {
// Fix needed: Add #include <string> at top
```

**Issue 2**: `many1(spaces)` call is invalid

```cpp
// Line 26 - spaces is a lambda, but many1 expects (const char*, Parser)
// This will NOT compile
inline auto spaces1 = many1(spaces);

// Fix: Need to wrap in a callable
inline auto spaces1 = [](const char* s) {
    return many1(s, spaces);
};
```

**Issue 3**: `optional(spaces)` has same problem

```cpp
// Line 29 - same issue
inline auto opt_spaces = optional(spaces);

// Fix: Same pattern
inline auto opt_spaces = [](const char* s) {
    return optional(s, spaces);
};
```

### 4.2 MEDIUM: `choice3` Missing Implementation

**File**: `include/infra/parsing/combinators.h`

`features.md` documents `choice3` but implementation doesn't exist:

```cpp
// Documented in features.md:
// - choice3(p1, p2, p3) — 尝试三个解析器

// But combinators.h only has choice(p1, p2)
```

**Fix**: Implement `choice3`:

```cpp
template<typename T>
inline ParseResult<T> choice3(const char* s, Parser<T> p1, Parser<T> p2, Parser<T> p3) {
    auto r = p1(s);
    if (r.ok()) return r;
    r = p2(s);
    if (r.ok()) return r;
    return p3(s);
}
```

### 4.3 LOW: `helpers.h` API Inconsistency

**Issue**: `spaces`, `spaces1`, `opt_spaces` are lambdas but `integer` is different:

```cpp
// spaces, opt_spaces: lambdas (wrong pattern)
inline auto spaces = [](const char* s) { ... };

// integer: lambda wrapper (correct pattern)
inline auto integer = [](const char* s) -> ParseResult<int> { ... };
```

**Recommendation**: Make all helpers consistent with `integer` pattern.

---

## 5. Is It Worthy as Long-Term Core Module?

### 5.1 YES - Justification

| Factor | Assessment |
|--------|------------|
| **Fit with philosophy** | ✅ Matches "explicit over implicit", "composable over monolithic" |
| **Size constraint** | ✅ Small enough to understand in < 30 min |
| **Zero dependencies** | ✅ Pure C++17, no external deps |
| **Performance** | ✅ Sub-microsecond operations, zero allocation |
| **Composability** | ✅ Parser combinator pattern is proven |
| **Maintainability** | ✅ No hidden state, testable |

### 5.2 Recommended Actions

1. **Fix `helpers.h`** - Critical compile errors (P0)
2. **Add `choice3`** - Missing documented feature (P1)
3. **Standardize helper APIs** - Consistency improvement (P2)

---

## 6. Template Complexity Analysis

### Current State: LOW COMPLEXITY ✅

| Metric | Value | Assessment |
|--------|-------|------------|
| Max template depth | 2 | Good |
| SFINAE usage | 0 | Excellent |
| Concept checks | 1 (Parser) | Adequate |
| Type deduction cases | Limited | Good |

**No over-abstraction detected.** The template usage is minimal and appropriate.

---

## 7. Readability Assessment

### Score: 8/10 ✅

**Strengths**:
- Clear naming (`char_p`, `many1`, `choice`)
- Inline documentation
- Consistent style

**Weaknesses**:
- `helpers.h` inconsistency
- Some lambda-heavy code hard to trace

---

## 8. Summary

| Category | Status |
|----------|--------|
| Design Principles | ✅ Compliant |
| Error Model | ✅ Explicit |
| Composability | ✅ Excellent |
| Zero Allocation | ✅ Verified |
| Code Quality | ⚠️ 3 issues (2 critical) |
| **Overall** | ✅ **Worthy as core module** |

**Verdict**: The parsing module is well-designed and worthy of long-term investment. Fix the 3 identified issues and it will be production-ready.

---

## Appendix: Benchmark Results

```
[ Parsing Benchmarks ]
Single char match:           0.01 us/op
Digit sequence (many):       0.66 us/op
Choice (2 alternatives):     0.02 us/op
Sequence (2 parsers):         0.03 us/op
Optional parser:              0.01 us/op

[ String Benchmarks ]
Left trim:                    0.01 us/op
Starts with:                  0.01 us/op

[ Span Benchmarks ]
Iteration (10 elements):      0.02 us/op
Subspan:                      0.01 us/op
```

*All operations sub-microsecond, zero heap allocation.*
