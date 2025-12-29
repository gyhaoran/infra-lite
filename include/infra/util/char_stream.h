#pragma once

#include "infra/parsing/result.h"

using infra::parsing::ParseResult;

namespace infra::util {

inline const char* skip_ws(const char* s) {
    while (*s == ' ' || *s == '\t' || *s == '\n') {
        ++s;
    }
    return s;
}

// Expect that the current character matches `expected`.
// On success: returns ParseResult with value == expected, next == s+1
// On failure: returns ParseResult with error == UnexpectedChar, next == s
inline ParseResult<char> expect_char(const char* s, char expected) {
    if (*s == expected) {
        return { expected, s + 1, infra::parsing::ParseError::None };
    } else {
        return ParseResult<char>::error_at(s, infra::parsing::ParseError::UnexpectedChar);
    }
}

} // namespace infra::util
