#pragma once

#include "parser.h"
#include "infra/util/char_stream.h"
#include <initializer_list>
#include <algorithm>

using infra::util::skip_ws;

namespace infra::parsing {

template<typename T>
ParseResult<T> chain_left(
    const char* s,
    Parser<T> operand,
    T (*apply)(char, T, T),
    std::initializer_list<char> ops
) {
    auto left = operand(s);
    if (!left.ok()) {
        return left;
    }

    while (true) {
        const char* p = skip_ws(left.next);
        char op = *p;

        if (std::find(ops.begin(), ops.end(), op) == ops.end()) {
            break;
        }

        auto right = operand(p + 1);
        if (!right.ok()) {
            return right;
        }

        T value = apply(op, left.value, right.value);
        left = { value, right.next, ParseError::None };
    }

    return left;
}

template<typename T>
ParseResult<T> chain_right(
    const char* s,
    Parser<T> operand,
    T (*apply)(T, T),
    std::initializer_list<char> ops
) {
    auto left = operand(s);
    if (!left.ok()) {
        return left;
    }

    const char* p = skip_ws(left.next);
    char op = *p;

    if (std::find(ops.begin(), ops.end(), op) == ops.end()) {
        return left;
    }

    auto right = chain_right<T>(p + 1, operand, apply, ops);
    if (!right.ok()) {
        return right;
    }

    T value = apply(left.value, right.value);
    return { value, right.next, ParseError::None };
}

} // namespace infra::parsing
