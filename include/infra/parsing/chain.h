
#pragma once

#include "parser.h"
#include "infra/util/char_stream.h"
#include <initializer_list>
#include <algorithm>

template<typename T>
ParseResult<T> chain_left(
    const char* s,
    Parser<T> operand,
    T (*apply)(char, T, T),
    std::initializer_list<char> ops
) {
    auto left = operand(s);
    while (true) {
        const char* p = skip_ws(left.next);
        char op = *p;

        if (std::find(ops.begin(), ops.end(), op) == ops.end()) {
            break;
        }
        auto right = operand(p + 1);
        T value = apply(op, left.value, right.value);
        left = { value, right.next };
    }
    return left;
}
