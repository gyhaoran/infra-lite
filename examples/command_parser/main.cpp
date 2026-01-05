#include "cmd_parser.h"
#include <cstdio>

int main() {
    const char* input =
        "run job1 --force timeout=30";

    auto r = parse_command(input);
    if (!r.ok()) {
        printf("Parse error\n");
        return 1;
    }

    printf("Command: %s\n", r.value.name.c_str());
    for (auto& arg : r.value.args) {
        if (arg.kind == Arg::Kind::Flag) {
            printf("  flag: --%s\n", arg.key.c_str());
        } else if (arg.kind == Arg::Kind::KeyValue) {
            printf("  kv: %s=%d\n", arg.key.c_str(), arg.int_value);
        } else {
            printf("  arg: %s\n", arg.key.c_str());
        }
    }
    return 0;
}
