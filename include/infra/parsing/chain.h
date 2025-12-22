
#pragma once
#include <initializer_list>
#include <algorithm>
#include "parser.h"

template<typename T>
ParseResult<T> chain_left(
    const char* s,
    Parser<T> operand,
    T (*apply)(char, T, T),
    std::initializer_list<char> ops
) {
    auto left = operand(s);
    char op = *left.next;

    while (std::find(ops.begin(), ops.end(), op) != ops.end()) {
        auto right = operand(left.next + 1);
        T value = apply(op, left.value, right.value);
        left = { value, right.next };
        op = *left.next;
    }
    return left;
}
