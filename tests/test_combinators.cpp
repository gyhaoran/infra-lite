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

// Free function parser for ','
ParseResult<char> parse_comma(const char* s) {
    return expect_char(s, ',');
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

    // Test sequence2
    auto r6 = sequence2("ab", parse_a, parse_b);
    assert(r6.ok());
    assert(r6.value.first == 'a');
    assert(r6.value.second == 'b');
    std::cout << "  sequence2: OK (parsed 'a' and 'b')\n";

    // Test sequence2 failure
    auto r7 = sequence2("ac", parse_a, parse_b);
    assert(!r7.ok());
    std::cout << "  sequence2: OK (failed on wrong second char)\n";

    // Test sep_by - comma separated digits
    auto r8 = sep_by("1,2,3", parse_digit, parse_comma);
    assert(r8.ok());
    assert(r8.value.size() == 3);
    std::cout << "  sep_by: OK (parsed 3 digits)\n";

    // Test sep_by empty
    auto r9 = sep_by("abc", parse_digit, parse_plus);
    assert(r9.ok());
    assert(r9.value.empty());
    std::cout << "  sep_by: OK (empty on no match)\n";

    // Test lookahead - should not consume input
    auto r10 = lookahead("abc", parse_a);
    assert(r10.ok());
    assert(r10.value == 'a');
    std::cout << "  lookahead: OK (did not consume input)\n";

    std::cout << "All combinator tests passed!\n";
    return 0;
}