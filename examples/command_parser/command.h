#pragma once

#include <string>
#include <vector>

struct Arg {
    enum class Kind {
        Ident,
        Flag,
        KeyValue
    };

    Kind kind;
    std::string key;
    int int_value = 0;
};

struct Command {
    std::string name;
    std::vector<Arg> args;
};
