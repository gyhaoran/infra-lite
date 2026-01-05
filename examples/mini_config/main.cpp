#include "parser.h"
#include <cstdio>

int main() {
    const char* text = "timeout = 30\n"
        "retry = 3\n"
        "debug\n";

    auto r = parse_config(text);
    if (!r.ok()) {
        printf("Parse error\n");
        return 1;
    }

    for (auto& item : r.value) {
        if (!item.has_value()) {
            printf("%s\n", item.key.c_str());
            continue;
        }

        if (item.is_int()) {
            printf("%s = %d\n", item.key.c_str(), item.as_int());
        } else if (item.is_string()) {
            printf("%s = %s\n", item.key.c_str(), item.as_string().c_str());
        }
    }
}
