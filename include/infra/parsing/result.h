#pragma once

#include "infra/error.h"

namespace infra::parsing {

// ============================================================================
// Parse Result (specialized for parsing context)
// ============================================================================

/// Parse-specific result type.
/// Extends infra::Result with parsing context (position tracking).
/// 
/// Parser contract:
///   - On success: ok() == true, value contains parsed result, next points to first unconsumed char
///   - On failure: ok() == false, error contains failure reason, next points to error position
///   - Parser MUST NOT skip trailing input validation

template<typename T>
struct ParseResult {
    T value{};
    const char* next = nullptr;
    Error error;

    /// Check if parsing succeeded.
    bool ok() const noexcept { return error.ok(); }

    /// Create a successful result.
    static constexpr ParseResult success(T val, const char* nxt) noexcept {
        ParseResult r;
        r.value = std::move(val);
        r.next = nxt;
        return r;
    }

    /// Create an error result.
    static constexpr ParseResult error_at(const char* pos, Error e) noexcept {
        ParseResult r;
        r.next = pos;
        r.error = e;
        return r;
    }

    /// Create an unexpected character error.
    static constexpr ParseResult unexpected_char(const char* pos) noexcept {
        ParseResult r;
        r.next = pos;
        r.error = Error{Error::UnexpectedChar};
        return r;
    }

    /// Create an unexpected end error.
    static constexpr ParseResult unexpected_end(const char* pos) noexcept {
        ParseResult r;
        r.next = pos;
        r.error = Error{Error::UnexpectedEnd};
        return r;
    }
};

/// Specialization for void (used when parser doesn't produce value).
template<>
struct ParseResult<void> {
    const char* next = nullptr;
    Error error;

    bool ok() const noexcept { return error.ok(); }

    static constexpr ParseResult success(const char* nxt) noexcept {
        ParseResult r;
        r.next = nxt;
        return r;
    }

    static constexpr ParseResult error_at(const char* pos, Error e) noexcept {
        ParseResult r;
        r.next = pos;
        r.error = e;
        return r;
    }

    static constexpr ParseResult unexpected_char(const char* pos) noexcept {
        ParseResult r;
        r.next = pos;
        r.error = Error{Error::UnexpectedChar};
        return r;
    }

    static constexpr ParseResult unexpected_end(const char* pos) noexcept {
        ParseResult r;
        r.next = pos;
        r.error = Error{Error::UnexpectedEnd};
        return r;
    }
};

} // namespace infra::parsing