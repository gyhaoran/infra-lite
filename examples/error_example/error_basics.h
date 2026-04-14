#pragma once

#include "infra/error.h"
#include <string_view>

namespace examples {

// ============================================================================
// Error Handling Examples
// ============================================================================

/// Demonstrates basic Error type usage.
/// Error is a simple value type with category information.
inline void demonstrate_error_basics() {
    // Default construction creates a "none" error
    infra::Error e1;  // kind = None, ok() = true
    
    // Explicit construction
    infra::Error e2(infra::Error::UnexpectedChar);  // kind = UnexpectedChar
    
    // With message
    infra::Error e3(infra::Error::Custom, "something went wrong");
    
    // Check state
    if (!e2.ok()) {
        // Handle error
    }
    
    // Boolean conversion
    if (e3) {
        // Error occurred
    }
}

/// Demonstrates Result<T> usage for fallible operations.
inline void demonstrate_result() {
    // Success case
    infra::Result<int> r1 = infra::make_ok(42);
    if (r1) {
        int val = *r1;           // via operator*
        int val2 = r1.value();   // via value()
    }
    
    // Error case - use explicit Error type
    infra::Result<int> r2 = infra::make_error<int>(infra::Error(infra::Error::Custom, "division by zero"));
    if (!r2.ok()) {
        infra::Error err = r2.error();
    }
    
    // map: transform the value
    auto doubled = r1.map([](int x) { return x * 2; });
    // doubled contains 84
    
    // and_then: chain operations
    auto result = r1.and_then([](int x) -> infra::Result<int> {
        if (x > 100) return infra::make_error<int>(infra::Error(infra::Error::Custom, "too large"));
        return infra::make_ok(x * 2);
    });
}

/// Demonstrates void Result for operations without return values.
inline void demonstrate_void_result() {
    infra::Result<void> r = infra::make_ok();
    if (r) {
        // Success
    }
    
    // and_then for void
    auto chained = r.and_then([]() -> infra::Result<void> {
        // Do something that might fail
        return infra::make_error<void>(infra::Error(infra::Error::Custom, "failed"));
    });
}

// ============================================================================
// Parsing Error Context
// ============================================================================

/// Example: Parsing function that returns Result<int>
infra::Result<int> parse_positive_int(std::string_view input) {
    if (input.empty()) {
        return infra::make_error<int>(infra::Error(infra::Error::UnexpectedEnd, "unexpected end of input"));
    }
    
    int value = 0;
    for (char c : input) {
        if (c < '0' || c > '9') {
            return infra::make_error<int>(infra::Error(infra::Error::UnexpectedChar, "invalid character"));
        }
        value = value * 10 + (c - '0');
    }
    
    return infra::make_ok(value);
}

}  // namespace examples