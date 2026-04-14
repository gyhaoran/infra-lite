#include <iostream>
#include <cstdio>

// Test that logging compiles and works when enabled
#define INFRA_LOG_ENABLED
#include "infra/logging.h"

int main() {
    std::cout << "Running logging module tests...\n";

    // Test that macros expand correctly
    INFRA_LOG_INFO("Test info message");
    INFRA_LOG_WARN("Test warn message");
    INFRA_LOG_ERROR("Test error message");
    INFRA_LOG_DEBUG("Test debug message");

    std::cout << "Logging module tests complete!\n";
    return 0;
}