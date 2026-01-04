#include "calculator.h"
#include "infra/parsing/chain.h"
#include "infra/parsing/primitives.h"
#include <cstdio>
#include <cmath>

using namespace infra::parsing;
using infra::util::skip_ws;

ParseResult<int> parse_add_sub(const char*);

int apply_op(char op, int l, int r) {
    switch (op) {
        case '+': { return l + r; }
        case '-': { return l - r; }
        case '*': { return l * r; }
        case '/': {
            if (r == 0) {
                std::fprintf(stderr, "Runtime error: division by zero\n");
                std::exit(1);
            }
            return l / r;
        }
    }
    return 0;
}

int apply_power(int base, int exp) {
    if (exp < 0) {
        std::fprintf(stderr, "Runtime error: negative exponent\n");
        std::exit(1);
    }
    return static_cast<int>(std::pow(base, exp));
}

ParseResult<int> parse_number(const char* s) {
    s = skip_ws(s);

    int v = 0;
    int len = 0;
    int matched = sscanf(s, "%d%n", &v, &len);

    if (matched != 1) {
        return ParseResult<int>::error_at(s, ParseError::UnexpectedChar);
    }

    return { v, s + len, ParseError::None };
}

ParseResult<int> parse_group(const char* s) {
    s = skip_ws(s);

    if (*s == '(') {
        auto inner = parse_add_sub(s + 1);
        if (!inner.ok()) {
            return inner;
        }

        const char* p = skip_ws(inner.next);
        auto close = char_p(p, ')');
        if (!close.ok()) { 
            return ParseResult<int>::error_at(close.next, close.error);
        }
        return { inner.value, close.next, ParseError::None };
    }

    return parse_number(s);
}

ParseResult<int> parse_unary(const char* s) {
    s = skip_ws(s);

    if (*s == '+' || *s == '-') {
        auto inner = parse_unary(s + 1);
        if (!inner.ok()) {
            return inner;
        }

        return (*s == '+')
            ? inner
            : ParseResult<int>{ -inner.value, inner.next, ParseError::None };
    }

    return parse_group(s);
}

ParseResult<int> parse_power(const char* s) {
    return chain_right<int>(s, parse_unary, apply_power, {'^'});
}

ParseResult<int> parse_mul_div(const char* s) {
    return chain_left<int>(s, parse_power, apply_op, {'*', '/'});
}

ParseResult<int> parse_add_sub(const char* s) {
    return chain_left<int>(s, parse_mul_div, apply_op, {'+', '-'});
}

int eval(const char* s) {
    return parse_add_sub(s).value;
}
