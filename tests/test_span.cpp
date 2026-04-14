#include <iostream>
#include <cassert>
#include <cstring>

#include "infra/util/span.h"

using namespace infra::util;

int main() {
    std::cout << "Running span tests...\n";

    // Test basic construction
    char arr[] = "hello";
    span<char> s1(arr, 5);
    assert(s1.size() == 5);
    assert(s1.data() == arr);
    std::cout << "  span construction: OK\n";

    // Test array initialization
    int int_arr[] = {1, 2, 3, 4, 5};
    span<int> s2(int_arr);
    assert(s2.size() == 5);
    std::cout << "  span from array: OK\n";

    // Test empty span
    span<char> s3;
    assert(s3.size() == 0);
    assert(s3.empty());
    std::cout << "  empty span: OK\n";

    // Test operator[]
    assert(s1[0] == 'h');
    assert(s1[4] == 'o');
    std::cout << "  operator[]: OK\n";

    // Test first/last
    auto first2 = s1.first(2);
    assert(first2.size() == 2);
    assert(first2[0] == 'h');
    assert(first2[1] == 'e');
    std::cout << "  first(): OK\n";

    auto last2 = s1.last(2);
    assert(last2.size() == 2);
    assert(last2[0] == 'l');
    assert(last2[1] == 'o');
    std::cout << "  last(): OK\n";

    // Test subspan
    auto sub = s1.subspan(1, 3);
    assert(sub.size() == 3);
    assert(sub[0] == 'e');
    assert(sub[2] == 'l');
    std::cout << "  subspan(): OK\n";

    // Test iterator
    int count = 0;
    for (auto c : s1) {
        ++count;
    }
    assert(count == 5);
    std::cout << "  iterator: OK\n";

    std::cout << "All span tests passed!\n";
    return 0;
}