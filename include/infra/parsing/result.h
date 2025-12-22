
#pragma once

template<typename T>
struct ParseResult {
    T value;
    const char* next;
};
