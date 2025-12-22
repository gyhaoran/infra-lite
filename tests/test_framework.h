#pragma once
#include <cstdio>

namespace test {

static int tests_run = 0;
static int tests_failed = 0;

inline void check(bool expr, const char* msg,
                  const char* file, int line) {
    ++tests_run;
    if (!expr) {
        ++tests_failed;
        std::printf("[FAILED] %s (%s:%d)\n", msg, file, line);
    }
}

inline int summary() {
    if (tests_failed == 0) {
        std::printf("[OK] All %d tests passed\n", tests_run);
        return 0;
    } else {
        std::printf("[ERROR] %d / %d tests failed\n",
                    tests_failed, tests_run);
        return 1;
    }
}

} // namespace test

#define TEST_CHECK(expr) \
    test::check((expr), #expr, __FILE__, __LINE__)
