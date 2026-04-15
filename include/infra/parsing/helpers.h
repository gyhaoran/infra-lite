#pragma once

#include <string>
#include <cctype>

#include "infra/parsing/combinators.h"
#include "infra/parsing/primitives.h"
#include "infra/util/string_utils.h"

namespace infra::parsing {

// ============================================================================
// Common Parser Helpers
// ============================================================================

/// Match whitespace characters.
inline auto spaces = char_if([](char c) { return c == ' ' || c == '\t' || c == '\n' || c == '\r'; });

/// Match zero or more whitespace (skip).
inline auto skip_spaces = [](const char* s) -> ParseResult<const char*> {
    const char* p = s;
    while (*p && (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')) {
        ++p;
    }
    return ParseResult<const char*>::success(s, p);
};

/// Match one or more whitespace.
inline auto spaces1 = [](const char* s) {
    return many1(s, spaces);
};

/// Match optional whitespace.
inline auto opt_spaces = [](const char* s) {
    return optional(s, spaces);
};

/// Match an identifier (alphanumeric + underscore, starting with letter).
inline auto identifier = [](const char* s) -> ParseResult<std::string> {
    s = infra::util::ltrim(s);
    if (!*s || (!std::isalpha(*s) && *s != '_')) {
        return ParseResult<std::string>::unexpected_char(s);
    }
    const char* start = s;
    while (std::isalnum(*s) || *s == '_') {
        ++s;
    }
    return ParseResult<std::string>::success(std::string(start, s), s);
};

/// Match an integer (with optional sign).
inline auto integer = [](const char* s) -> ParseResult<int> {
    s = infra::util::ltrim(s);
    const char* start = s;

    // Optional sign
    if (*s == '+' || *s == '-') {
        ++s;
    }

    // Must have at least one digit
    if (!std::isdigit(*s)) {
        return ParseResult<int>::unexpected_char(start);
    }

    int value = 0;
    while (std::isdigit(*s)) {
        value = value * 10 + (*s - '0');
        ++s;
    }

    if (*start == '-') {
        value = -value;
    }

    return ParseResult<int>::success(value, s);
};

}  // namespace infra::parsing