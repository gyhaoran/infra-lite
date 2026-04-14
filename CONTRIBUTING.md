# Contributing to infra-lite

Thank you for your interest in contributing to infra-lite! This document outlines the development guidelines, design principles, and commit conventions.

---

## Core Philosophy

infra-lite follows the "anti-framework" principle:
- **Infrastructure over features** — Foundation before convenience
- **Simple over complete** — Do fewer things, do them well
- **Explicit over magic** — No hidden state
- **Composable over monolithic** — Small, reusable pieces

**This project will NEVER become a bloated "everything library".**

---

## Design Principles

### 1. Clean Code

- **Readable** — Code should be self-documenting
- **Minimal** — No unnecessary complexity
- **Focused** — Single responsibility per function/class

```cpp
// Good: Clear intent
auto parse_number = many1(char_p(isdigit));

// Bad: Obscured intent
template<typename T, 
         template<typename> class Parser,
         typename Enable = void>
struct NumberParser { ... };
```

### 2. SOLID Principles (Adapted for C++)

| Principle | Application |
|-----------|-------------|
| **S**ingle Responsibility | Each module does one thing well |
| **O**pen/Closed | Extend via composition, not modification |
| **L**iskov Substitution | Subtypes must be substitutable |
| **I**nterface Segregation | Small, focused interfaces |
| **D**ependency Inversion | Depend on abstractions, not concretions |

### 3. RAII (Resource Acquisition Is Initialization)

- All resources must be managed by objects with deterministic lifetimes
- No manual resource cleanup
- Use `std::unique_ptr` for heap-allocated resources when needed

```cpp
// Good: RAII-compliant
class Parser {
    std::vector<char> buffer_;  // RAII-managed
public:
    ~Parser() = default;  // Automatic cleanup
};

// Bad: Manual cleanup required
class Parser {
    char* buffer_;
public:
    ~Parser() { free(buffer_); }  // Error-prone
};
```

### 4. Const Correctness

- Mark functions `const` when they don't modify state
- Use `const` references to avoid unnecessary copies
- Prefer `const` iterators

```cpp
// Good
class Result {
public:
    bool ok() const { return !error_; }
    const char* where() const { return pos_; }
};

// Bad
class Result {
public:
    bool ok() { return !error_; }  // Missing const
};
```

### 5. Noexcept Strategy

- Mark functions `noexcept` when they cannot throw
- Parsing primitives should be `noexcept` (no allocation)
- Error handling functions should be `noexcept`
- Only throw if absolutely necessary (prefer `error` return types)

```cpp
// Good: noexcept parsing
constexpr auto char_p(char expected) noexcept {
    return [expected](const char* input) noexcept -> ParseResult<char> {
        // ...
    };
}
```

---

## Prohibited Patterns

### ❌ Raw Pointers (Unless Explicit Justification)

Raw pointers create ownership ambiguity. Use:
- `std::unique_ptr` for exclusive ownership
- `std::shared_ptr` for shared ownership
- References for non-owning access

```cpp
// Bad
class Parser {
    char* buffer_;  // Who owns this?
};

// Good
class Parser {
    std::vector<char> buffer_;  // Clear ownership
    std::span<const char> input_;  // Non-owning view
};
```

**Exception**: Interfacing with C libraries that require raw pointers.

### ❌ Macro Abuse

Macros are:
- Undebuggable
- Untype-safe
- Hard to reason about

Prefer:
- `constexpr` functions
- `inline` functions
- Templates

```cpp
// Bad
#define MAX(a, b) ((a) > (b) ? (a) : (b))

// Good
template<typename T>
constexpr const T& max(const T& a, const T& b) noexcept {
    return a > b ? a : b;
}
```

**Exception**: Include guards (conventional and necessary).

### ❌ Implicit Allocation

Every allocation should be:
- Explicit
- Visible in the call site
- Documented in the interface

```cpp
// Bad: Hidden allocation
auto parse_string() {
    return std::make_unique<std::string>();  // Surprise allocation!
}

// Good: Explicit allocation
auto parse_string(std::string& out) -> bool;  // Caller provides buffer

// Good: Zero-allocation
auto parse_string(std::span<const char> input) -> std::string_view;  // Views input
```

---

## Code Style

### Naming Conventions

| Element | Convention | Example |
|---------|------------|---------|
| Types/Classes | PascalCase | `ParseResult`, `CharStream` |
| Functions | snake_case | `parse_number()`, `skip_whitespace()` |
| Constants | kCamelCase | `kDefaultBufferSize` |
| Member variables | snake_case_ | `input_pos_`, `error_kind_` |
| Template parameters | PascalCase | `T`, `TResult` |

### Header Organization

```cpp
// infra/example.h
#ifndef INFRA_EXAMPLE_H
#define INFRA_EXAMPLE_H

// [1] Headers (grouped: associated, third-party, standard)
// [2] Forward declarations
// [3] Main content

namespace infra {

// [4] Implementation details at bottom

}  // namespace infra

#endif  // INFRA_EXAMPLE_H
```

### File Structure

- One class/type per header file
- Implementation in `.h` for header-only modules
- Use `.cpp` only when separate compilation is needed

---

## Commit Conventions

### Format

```
<type>(<scope>): <short description>

[optional body]

[optional footer]
```

### Types

| Type | Description |
|------|-------------|
| `feat` | New feature |
| `fix` | Bug fix |
| `refactor` | Code restructuring (no behavior change) |
| `docs` | Documentation only |
| `test` | Adding or updating tests |
| `chore` | Maintenance tasks |

### Examples

```bash
# Good commits
git commit -m "feat(parser): add optional combinator"
git commit -m "fix(error): correct unexpected_char message"
git commit -m "refactor(core): remove hidden state in result"
git commit -m "docs: update README with quick start guide"

# Bad commits (will be rejected)
git commit -m "update"
git commit -m "fix bug"
git commit -m "asdfgh"
```

### Commit Requirements

1. **Single responsibility** — One commit, one change
2. **Compilable** — Each commit must compile
3. **Testable** — Core logic changes should include tests
4. **Revertable** — Must be safely revertable

---

## Development Workflow

### 1. Small Steps (Mandatory)

- Commit after every minimal change
- Each commit should be self-contained
- Never "batch" multiple unrelated changes

### 2. Local Only

**🚫 Forbidden operations:**
- `git push`
- `git pull`
- Any remote operations

You are responsible only for building **high-quality local commit history**.

### 3. Test Synchronization

For core logic changes:
- Add or update tests alongside the change
- Can be split: `feat` + `test` commits

### 4. Review Process

Before submitting:
- [ ] Code follows style guidelines
- [ ] No prohibited patterns
- [ ] Tests added/updated
- [ ] Commit message follows convention
- [ ] Header guards present
- [ ] `noexcept` used appropriately

---

## Module Design Guidelines

### Zero-Dependency Core

Core modules should:
- Have no external dependencies
- Use only standard C++ constructs
- Be header-only when possible

### Explicit Contracts

Every function should document:
- Preconditions
- Postconditions
- Error conditions
- Exception safety

```cpp
// Example contract
/// Parses a single digit character.
/// @param input Pointer to current position (must not be nullptr)
/// @return ParseResult with digit value on success, error on failure
/// @note Does not consume input on failure
constexpr auto digit_p(const char* input) noexcept -> ParseResult<char>;
```

### Error Handling

- Use `Result<T, E>` pattern for fallible operations
- Avoid exceptions for expected failures
- Reserve exceptions for truly exceptional conditions

---

## Documentation Requirements

### For Every Public API

```cpp
/// Brief description (one line).
///
/// Detailed description if needed.
///
/// @param param_name Description of parameter
/// @return Description of return value
/// @note Important notes or caveats
/// @see Related function or type
```

---

## Versioning Policy

infra-lite follows semantic versioning (MAJOR.MINOR.PATCH):
- **MAJOR**: Breaking changes to public API
- **MINOR**: New functionality (backward compatible)
- **PATCH**: Bug fixes (backward compatible)

---

## Questions?

Open an issue for discussion before making significant changes.

---

*Last updated: 2026-04-14*