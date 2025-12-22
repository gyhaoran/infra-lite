#include "calculator.h"
#include <cstdio>

int main() {
    const char* expr = "1 + 2 * (3 + 4)";
    printf("%s = %d\n", expr, eval(expr));
}
