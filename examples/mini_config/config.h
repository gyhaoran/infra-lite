#pragma once

#include <string>
#include <vector>

struct ConfigItem {
    std::string key;
    bool has_value = false;
    int value = 0;
};

using Config = std::vector<ConfigItem>;
