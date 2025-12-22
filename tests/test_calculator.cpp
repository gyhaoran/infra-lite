#include "test_framework.h"

int eval(const char* expr);

int main() {
    TEST_CHECK(eval("1+2") == 3);
    TEST_CHECK(eval("1+2*3") == 7);
    TEST_CHECK(eval("(1+2)*3") == 9);
    TEST_CHECK(eval("-3+5") == 2);
    TEST_CHECK(eval("1 + 2 * (3 + 4)") == 15);
    return test::summary();
}
