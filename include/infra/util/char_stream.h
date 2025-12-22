#pragma once

inline const char* skip_ws(const char* s) {
    while (*s == ' ' || *s == '\t' || *s == '\n') {
        ++s;
    }
    return s;
}
