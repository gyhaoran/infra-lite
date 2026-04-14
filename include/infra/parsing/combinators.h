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

// ============================================================================
// Choice Combinators
// ============================================================================

/// choice(p1, p2) - Try two parsers, return first success.
/// Returns first error if both fail.
template<typename T>
inline ParseResult<T> choice(const char* s, Parser<T> p1, Parser<T> p2) {
    auto r1 = p1(s);
    if (r1.ok()) {
        return r1;
    }
    auto r2 = p2(s);
    if (r2.ok()) {
        return r2;
    }
    // Both failed, return first error
    return ParseResult<T>::error_at(s, r1.error);
}

/// choice3(p1, p2, p3) - Try three parsers, return first success.
template<typename T>
inline ParseResult<T> choice3(const char* s, Parser<T> p1, Parser<T> p2, Parser<T> p3) {
    auto r = choice(s, p1, p2);
    if (r.ok()) {
        return r;
    }
    return p3(s);
}

} // namespace infra::parsing