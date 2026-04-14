#include <iostream>
#include <cassert>
#include <cctype>

#include "infra/util/string_utils.h"

using namespace infra::util;

int main() {
    std::cout << "Running string utilities tests...\n";

    // Test ltrim
    const char* s1 = "   hello";
    const char* r1 = ltrim(s1);
    assert(r1 == s1 + 3);
    std::cout << "  ltrim: OK\n";

    // Test starts_with
    assert(starts_with("hello world", "hello"));
    assert(!starts_with("world hello", "hello"));
    std::cout << "  starts_with: OK\n";

    // Test ends_with
    assert(ends_with("hello.cpp", ".cpp"));
    assert(!ends_with("hello.h", ".cpp"));
    std::cout << "  ends_with: OK\n";

    // Test find_char
    const char* s2 = "hello";
    const char* r2 = find_char(s2, 'l');
    assert(r2 == s2 + 2);
    std::cout << "  find_char: OK\n";

    // Test strcmp_view
    assert(strcmp_view("abc", "abc") == 0);
    assert(strcmp_view("abc", "abd") < 0);
    assert(strcmp_view("abd", "abc") > 0);
    std::cout << "  strcmp_view: OK\n";

    // Test strcasecmp_view
    assert(strcasecmp_view("ABC", "abc") == 0);
    assert(strcasecmp_view("ABC", "ABD") < 0);
    std::cout << "  strcasecmp_view: OK\n";

    std::cout << "All string utility tests passed!\n";
    return 0;
}