#include <iostream>
#include <cassert>

#include "infra/error.h"

using namespace infra;

int main() {
    std::cout << "Running error module tests...\n";

    // Test Error type
    Error e1;
    assert(e1.ok());
    std::cout << "  Error default: OK\n";

    Error e2(Error::UnexpectedChar);
    assert(!e2.ok());
    std::cout << "  Error UnexpectedChar: OK\n";

    Error e3(Error::UnexpectedEnd);
    assert(!e3.ok());
    std::cout << "  Error UnexpectedEnd: OK\n";

    Error e4(Error::Custom, "custom error");
    assert(!e4.ok());
    std::cout << "  Error Custom: OK\n";

    // Test Result<T>
    Result<int> r1 = make_ok(42);
    assert(r1.ok());
    assert(*r1 == 42);
    std::cout << "  Result<int> success: OK\n";

    Result<int> r2 = make_error<int>(Error(Error::Custom, "failed"));
    assert(!r2.ok());
    std::cout << "  Result<int> error: OK\n";

    // Test map
    auto r3 = r1.map([](int x) { return x * 2; });
    assert(r3.ok());
    assert(*r3 == 84);
    std::cout << "  Result map: OK\n";

    // Test Result<void>
    Result<void> r4 = make_ok();
    assert(r4.ok());
    std::cout << "  Result<void> success: OK\n";

    Result<void> r5 = make_error<void>(Error(Error::Custom, "void error"));
    assert(!r5.ok());
    std::cout << "  Result<void> error: OK\n";

    std::cout << "All error module tests passed!\n";
    return 0;
}