#pragma once

#include <string>
#include <vector>

struct ConfigItem {
    std::string key;

    enum class ValueType {
        None,
        Int,
        String
    };

    ValueType type = ValueType::None;

    int int_value = 0;
    std::string str_value;

    // ---- value accessors ----

    bool has_value() const {
        return type != ValueType::None;
    }

    bool is_int() const {
        return type == ValueType::Int;
    }

    bool is_string() const {
        return type == ValueType::String;
    }

    int as_int() const {
        return int_value;
    }

    const std::string& as_string() const {
        return str_value;
    }
};


using Config = std::vector<ConfigItem>;
