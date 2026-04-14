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

// ============================================================================
// Sequence Combinators
// ============================================================================

/// sequence2(p1, p2) - Run two parsers in sequence, return pair of results.
/// Fails if either parser fails.
template<typename T1, typename T2>
inline ParseResult<std::pair<T1, T2>> sequence2(const char* s, Parser<T1> p1, Parser<T2> p2) {
    auto r1 = p1(s);
    if (!r1.ok()) {
        return ParseResult<std::pair<T1, T2>>::error_at(s, r1.error);
    }
    auto r2 = p2(r1.next);
    if (!r2.ok()) {
        return ParseResult<std::pair<T1, T2>>::error_at(r1.next, r2.error);
    }
    return ParseResult<std::pair<T1, T2>>::success({std::move(r1.value), std::move(r2.value)}, r2.next);
}

/// then(p, f) - Run parser p, then apply function f to result.
/// Useful for transforming parsed values.
/// 
/// Example: parse number then apply double
///   auto doubled = then(parse_number, [](int n) { return n * 2; });
template<typename T, typename F>
inline auto then(const char* s, Parser<T> p, F&& f) 
    -> ParseResult<std::invoke_result_t<F, T>> {
    auto r = p(s);
    if (!r.ok()) {
        return ParseResult<std::invoke_result_t<F, T>>::error_at(s, r.error);
    }
    return ParseResult<std::invoke_result_t<F, T>>::success(f(std::move(r.value)), r.next);
}

} // namespace infra::parsing