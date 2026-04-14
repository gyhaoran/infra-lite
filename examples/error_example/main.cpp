#include "error_basics.h"
#include <iostream>

int main() {
    std::cout << "=== Error Handling Examples ===\n\n";
    
    // Basic Error usage
    std::cout << "1. Basic Error type:\n";
    examples::demonstrate_error_basics();
    std::cout << "   - Error is a simple value type\n";
    std::cout << "   - Supports None, UnexpectedChar, UnexpectedEnd, Custom kinds\n\n";
    
    // Result<T> usage
    std::cout << "2. Result<T> for fallible operations:\n";
    examples::demonstrate_result();
    std::cout << "   - make_ok() creates success result\n";
    std::cout << "   - make_error() creates error result\n";
    std::cout << "   - map() transforms value\n";
    std::cout << "   - and_then() chains operations\n\n";
    
    // Parsing example
    std::cout << "3. Parsing with Result:\n";
    auto r = examples::parse_positive_int("12345");
    if (r) {
        std::cout << "   Parsed: " << *r << "\n";
    }
    
    auto r2 = examples::parse_positive_int("abc");
    if (!r2) {
        std::cout << "   Parse failed: " << r2.error().message << "\n";
    }
    
    std::cout << "\n=== All examples completed ===\n";
    return 0;
}