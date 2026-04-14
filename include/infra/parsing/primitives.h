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

// ============================================================================
// Parser Factory Functions
// ============================================================================

/// char_p(c) - Create parser that matches exactly character c.
/// Usage: auto parser = char_p('+');
inline auto char_p(char expected) {
    return [expected](const char* s) noexcept -> ParseResult<char> {
        return expect_char(s, expected);
    };
}

/// char_if(pred) - Create parser that matches if predicate returns true.
/// Usage: auto digit = char_if(::isdigit);
template<typename Pred>
inline auto char_if(Pred pred) {
    return [pred](const char* s) noexcept -> ParseResult<char> {
        return satisfy(s, pred);
    };
}

/// one_of(chars) - Create parser that matches any char in string.
/// Usage: auto op = one_of("+-*/");
inline auto one_of(const char* chars) {
    return [=](const char* s) noexcept -> ParseResult<char> {
        if (*s == '\0') {
            return ParseResult<char>::unexpected_end(s);
        }
        for (const char* p = chars; *p; ++p) {
            if (*s == *p) {
                return ParseResult<char>::success(*s, s + 1);
            }
        }
        return ParseResult<char>::unexpected_char(s);
    };
}

/// none_of(chars) - Create parser that matches any char NOT in string.
/// Usage: auto ident_char = none_of(" \t\n");
inline auto none_of(const char* chars) {
    return [=](const char* s) noexcept -> ParseResult<char> {
        if (*s == '\0') {
            return ParseResult<char>::unexpected_end(s);
        }
        for (const char* p = chars; *p; ++p) {
            if (*s == *p) {
                return ParseResult<char>::unexpected_char(s);
            }
        }
        return ParseResult<char>::success(*s, s + 1);
    };
}

} // namespace infra::parsing