#pragma once

#include "result.h"

namespace infra::parsing {

/// Expect a specific character.
/// @return Parsed char on success, error on unexpected end or mismatch.
inline ParseResult<char> expect_char(const char* s, char expected) {
    if (*s == '\0') {
        return ParseResult<char>::unexpected_end(s);
    }
    if (*s != expected) {
        return ParseResult<char>::unexpected_char(s);
    }
    return ParseResult<char>::success(expected, s + 1);
}

/// Consume any single character.
/// @return Parsed char on success, error on unexpected end.
inline ParseResult<char> any_char(const char* s) {
    if (*s == '\0') {
        return ParseResult<char>::unexpected_end(s);
    }
    return ParseResult<char>::success(*s, s + 1);
}

/// Consume a character if predicate returns true.
/// @param pred Function: char -> bool
/// @return Parsed char on success, error if predicate fails or end of input.
template<typename Pred>
inline ParseResult<char> satisfy(const char* s, Pred pred) {
    if (*s == '\0') {
        return ParseResult<char>::unexpected_end(s);
    }
    if (!pred(*s)) {
        return ParseResult<char>::unexpected_char(s);
    }
    return ParseResult<char>::success(*s, s + 1);
}

} // namespace infra::parsing