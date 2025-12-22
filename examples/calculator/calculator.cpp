#include "calculator.h"
#include "infra/parsing/chain.h"
#include "infra/util/char_stream.h"
#include <cstdio>

ParseResult<int> parse_add_sub(const char*);

int apply_op(char op, int l, int r) {
    switch (op) {
        case '+': return l + r;
        case '-': return l - r;
        case '*': return l * r;
        case '/': return l / r;
    }
    return 0;
}

ParseResult<int> parse_number(const char* s) {
    s = skip_ws(s);
    int v, len;
    sscanf(s, "%d%n", &v, &len);
    return {v, s + len};
}

ParseResult<int> parse_group(const char* s) {
    s = skip_ws(s);
    if (*s == '(') {
        auto inner = parse_add_sub(s + 1);
        return {inner.value, inner.next + 1};
    }
    return parse_number(s);
}

ParseResult<int> parse_unary(const char* s) {
    if (*s == '+' || *s == '-') {
        auto inner = parse_unary(s + 1);
        return (*s == '+') ? inner : ParseResult<int>{-inner.value, inner.next};
    }
    return parse_group(s);
}

ParseResult<int> parse_mul_div(const char* s) {
    return chain_left<int>(s, parse_unary, apply_op, {'*', '/'});
}

ParseResult<int> parse_add_sub(const char* s) {
    return chain_left<int>(s, parse_mul_div, apply_op, {'+', '-'});
}

int eval(const char* s) {
    return parse_add_sub(s).value;
}
