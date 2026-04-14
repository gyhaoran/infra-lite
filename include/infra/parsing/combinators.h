#pragma once

#include "parser.h"
#include "result.h"

#include <vector>

namespace infra::parsing {

/// optional(parser)
/// - If parser succeeds: return its result
/// - If parser fails: return success with no consumption
template<typename T>
inline ParseResult<T> optional(const char* s, Parser<T> parser) {
    auto r = parser(s);
    if (r.ok()) {
        return r;
    }
    // swallow error, do not consume input
    return ParseResult<T>::success(T{}, s);
}

/// lookahead(parser)
/// - Run parser, do NOT consume input
/// - Only propagate success / error
template<typename T>
inline ParseResult<T> lookahead(const char* s, Parser<T> parser) {
    auto r = parser(s);
    if (!r.ok()) {
        return r;
    }
    // success, but do not advance
    return ParseResult<T>::success(r.value, s);
}

// ============================================================================
// Repetition Combinators
// ============================================================================

/// many(parser) - Match zero or more occurrences.
/// Returns vector of parsed values.
/// Always succeeds (even with zero matches).
/// 
/// @note For one-or-more, use many1() instead.
template<typename T>
inline ParseResult<std::vector<T>> many(const char* s, Parser<T> parser) {
    std::vector<T> results;
    const char* pos = s;

    while (true) {
        auto r = parser(pos);
        if (!r.ok()) {
            break;
        }
        results.push_back(std::move(r.value));
        pos = r.next;
    }

    return ParseResult<std::vector<T>>::success(std::move(results), pos);
}

/// many1(parser) - Match one or more occurrences.
/// Returns vector of parsed values.
/// Fails if no matches found.
/// 
/// @note For zero-or-more, use many() instead.
template<typename T>
inline ParseResult<std::vector<T>> many1(const char* s, Parser<T> parser) {
    // Try first match
    auto first = parser(s);
    if (!first.ok()) {
        return ParseResult<std::vector<T>>::error_at(s, first.error);
    }

    std::vector<T> results;
    results.push_back(std::move(first.value));
    const char* pos = first.next;

    // Continue matching
    while (true) {
        auto r = parser(pos);
        if (!r.ok()) {
            break;
        }
        results.push_back(std::move(r.value));
        pos = r.next;
    }

    return ParseResult<std::vector<T>>::success(std::move(results), pos);
}

} // namespace infra::parsing