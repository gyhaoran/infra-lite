#pragma once

#include <type_traits>
#include <cassert>
#include <utility>
#include <functional>

namespace infra {

// ============================================================================
// Error Types
// ============================================================================

/// Base error type with category information.
/// Errors are simple value types with no dynamic allocation.
struct Error {
    enum Kind {
        None = 0,
        UnexpectedChar,
        UnexpectedEnd,
        Custom,
    };

    Kind kind = None;
    const char* message = nullptr;

    constexpr Error() noexcept = default;
    constexpr explicit Error(Kind k) noexcept : kind(k) {}
    constexpr Error(Kind k, const char* msg) noexcept : kind(k), message(msg) {}

    explicit operator bool() const noexcept { return kind != None; }
    bool ok() const noexcept { return kind == None; }
};

/// Convenience constructors for common error types.
[[nodiscard]] constexpr Error none() noexcept { return Error{}; }

[[nodiscard]] constexpr Error unexpected_char(char, char) noexcept {
    return Error{Error::UnexpectedChar, nullptr};
}

[[nodiscard]] constexpr Error unexpected_end() noexcept {
    return Error{Error::UnexpectedEnd, "unexpected end of input"};
}

// ============================================================================
// Result Type (monad-like, C++17 compatible)
// ============================================================================

/// Result type for fallible operations.
/// 
/// Usage:
///   Result<int> divide(int a, int b) {
///     if (b == 0) return make_error("division by zero");
///     return a / b;
///   }
///
///   auto r = divide(10, 2);
///   if (r) { use(*r); }
///else { handle_error(r.error()); }
///
/// @tparam T Value type (must be nothrow-move-constructible for optimal usage)
template<typename T>
class Result {
public:
    using value_type = T;
    using error_type = Error;

private:
    union Storage {
        char dummy{};
        T value;
    } storage_;
    bool has_value_ = false;
    Error error_;

public:
    // Construction from value
    template<typename U = T, typename = std::enable_if_t<!std::is_same_v<U, void>>>
    constexpr Result(U&& v) noexcept(std::is_nothrow_move_constructible_v<T>)
        : storage_(), has_value_(true) {
        new (&storage_.value) T(std::move(v));
    }

    // Construction from error
    constexpr Result(Error e) noexcept : storage_(), has_value_(false), error_(e) {}

    // Destructor
    ~Result() noexcept {
        if (has_value_) {
            storage_.value.~T();
        }
    }

    // Move constructor
    Result(Result&& other) noexcept(std::is_nothrow_move_constructible_v<T>)
        : has_value_(other.has_value_), error_(other.error_) {
        if (has_value_) {
            new (&storage_.value) T(std::move(other.storage_.value));
        }
    }

    // Copy constructor (only if T is copyable)
    template<typename U = T, typename = std::enable_if_t<std::is_copy_constructible_v<U>>>
    Result(const Result& other) : has_value_(other.has_value_), error_(other.error_) {
        if (has_value_) {
            new (&storage_.value) T(other.storage_.value);
        }
    }

    // Move assignment
    Result& operator=(Result&& other) noexcept(std::is_nothrow_move_assignable_v<T>) {
        if (this != &other) {
            if (has_value_) {
                storage_.value.~T();
            }
            has_value_ = other.has_value_;
            error_ = other.error_;
            if (has_value_) {
                new (&storage_.value) T(std::move(other.storage_.value));
            }
        }
        return *this;
    }

    // Copy assignment (only if T is copyable)
    template<typename U = T, typename = std::enable_if_t<std::is_copy_assignable_v<U>>>
    Result& operator=(const Result& other) {
        if (this != &other) {
            if (has_value_) {
                storage_.value.~T();
            }
            has_value_ = other.has_value_;
            error_ = other.error_;
            if (has_value_) {
                new (&storage_.value) T(other.storage_.value);
            }
        }
        return *this;
    }

    // Observers
    [[nodiscard]] bool ok() const noexcept { return has_value_; }
    explicit operator bool() const noexcept { return has_value_; }

    [[nodiscard]] T& value() & {
        assert(ok() && "Result has no value");
        return storage_.value;
    }

    [[nodiscard]] const T& value() const & {
        assert(ok() && "Result has no value");
        return storage_.value;
    }

    [[nodiscard]] T&& value() && {
        assert(ok() && "Result has no value");
        return std::move(storage_.value);
    }

    [[nodiscard]] const T&& value() const && {
        assert(ok() && "Result has no value");
        return std::move(storage_.value);
    }

    [[nodiscard]] Error error() const noexcept { return error_; }

    // Operator* for optional-like syntax
    [[nodiscard]] T& operator*() { return value(); }
    [[nodiscard]] const T& operator*() const { return value(); }

    // Operator-> for pointer-like access to members
    [[nodiscard]] T* operator->() { 
        assert(ok() && "Result has no value");
        return &storage_.value;
    }
    [[nodiscard]] const T* operator->() const { 
        assert(ok() && "Result has no value");
        return &storage_.value;
    }

    // map: transform the value if present
    template<typename F>
    [[nodiscard]] auto map(F&& f) const noexcept 
        -> Result<std::invoke_result_t<F, T>> {
        using U = std::invoke_result_t<F, T>;
        if (ok()) {
            return Result<U>(std::forward<F>(f)(storage_.value));
        }
        return error_;
    }

    // and_then: chain operations that return Result
    template<typename F>
    [[nodiscard]] auto and_then(F&& f) const noexcept 
        -> std::invoke_result_t<F, T> {
        if (ok()) {
            return std::forward<F>(f)(storage_.value);
        }
        return error_;
    }
};

// ============================================================================
// Void Result Specialization
// ============================================================================

template<>
class Result<void> {
public:
    using value_type = void;
    using error_type = Error;

private:
    bool has_value_ = true;
    Error error_;

public:
    constexpr Result() noexcept = default;
    constexpr explicit Result(Error e) noexcept : has_value_(false), error_(e) {}

    [[nodiscard]] bool ok() const noexcept { return has_value_; }
    explicit operator bool() const noexcept { return has_value_; }
    [[nodiscard]] Error error() const noexcept { return error_; }

    template<typename F>
    [[nodiscard]] auto and_then(F&& f) const noexcept 
        -> std::invoke_result_t<F> {
        if (ok()) {
            return std::forward<F>(f)();
        }
        return error_;
    }
};

// ============================================================================
// Utility Functions
// ============================================================================

/// Create a success Result from a value.
template<typename T>
[[nodiscard]] constexpr Result<std::decay_t<T>> make_ok(T&& value) noexcept(
    std::is_nothrow_constructible_v<Result<std::decay_t<T>>, T&&>) {
    return Result<std::decay_t<T>>(std::forward<T>(value));
}

/// Create a success Result for void.
[[nodiscard]] constexpr Result<void> make_ok() noexcept {
    return Result<void>();
}

/// Create an error Result.
[[nodiscard]] constexpr Result<void> make_error(Error e) noexcept {
    return Result<void>(e);
}

/// Create an error Result with custom message.
[[nodiscard]] constexpr Result<void> make_error(const char* msg) noexcept {
    return Result<void>(Error{Error::Custom, msg});
}

} // namespace infra