#pragma once

#include "parser.h"
#include "result.h"

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

} // namespace infra::parsing