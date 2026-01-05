#pragma once
#include "result.h"

namespace infra::parsing {

inline ParseResult<char> expect_char(const char* s, char expected) {
    if (*s == '\0') {
        return ParseResult<char>::error_at(s, ParseError::UnexpectedEnd);
    }
    if (*s != expected) {
        return ParseResult<char>::error_at(s, ParseError::UnexpectedChar);
    }
    return { expected, s + 1, ParseError::None };
}

inline ParseResult<char> any_char(const char* s) {
    if (*s == '\0') {
        return ParseResult<char>::error_at(s, ParseError::UnexpectedEnd);
    }
    return { *s, s + 1, ParseError::None };
}

template <typename Pred>
inline ParseResult<char> satisfy(const char* s, Pred pred) {
    if (*s == '\0') {
        return ParseResult<char>::error_at(s, ParseError::UnexpectedEnd);
    }
    if (!pred(*s)) {
        return ParseResult<char>::error_at(s, ParseError::UnexpectedChar);
    }
    return { *s, s + 1, ParseError::None };
}

} // namespace infra::parsing
