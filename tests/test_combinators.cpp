#include <iostream>
#include <cassert>
#include <cctype>

#include "infra/parsing/combinators.h"
#include "infra/parsing/primitives.h"

using namespace infra::parsing;

// Test many1 - should match multiple digits
ParseResult<int> parse_digit(const char* s) {
    if (!std::isdigit(*s)) {
        return ParseResult<int>::unexpected_char(s);
    }
    return ParseResult<int>::success(*s - '0', s + 1);
}

// Free function parser for 'a'
ParseResult<char> parse_a(const char* s) {
    return expect_char(s, 'a');
}

// Free function parser for 'b'
ParseResult<char> parse_b(const char* s) {
    return expect_char(s, 'b');
}

// Free function parser for '+'
ParseResult<char> parse_plus(const char* s) {
    return expect_char(s, '+');
}

int main() {
    std::cout << "Running combinator tests...\n";

    // Test many1
    auto r1 = many1("123", parse_digit);
    assert(r1.ok());
    assert(r1.value.size() == 3);
    std::cout << "  many1: OK (parsed " << r1.value.size() << " digits)\n";

    // Test choice
    auto r2 = choice("a", parse_a, parse_b);
    assert(r2.ok());
    assert(r2.value == 'a');
    std::cout << "  choice: OK (matched 'a')\n";

    auto r3 = choice("b", parse_a, parse_b);
    assert(r3.ok());
    assert(r3.value == 'b');
    std::cout << "  choice: OK (matched 'b')\n";

    // Test skip
    auto r4 = skip("+abc", parse_plus);
    assert(r4.ok());
    assert(r4.value == "+abc");  // Returns original position
    std::cout << "  skip: OK (returned original position)\n";

    // Test optional - should fail and return default
    auto r5 = optional("xyz", parse_a);
    assert(r5.ok());
    std::cout << "  optional: OK (returned default on failure)\n";

    std::cout << "All combinator tests passed!\n";
    return 0;
}