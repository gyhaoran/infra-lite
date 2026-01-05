#pragma once

#include "infra/parsing/result.h"

using infra::parsing::ParseResult;

namespace infra::util {

inline const char* skip_ws(const char* s) {
    while (*s == ' ' || *s == '\t' || *s == '\n') {
        ++s;
    }
    return s;
}

} // namespace infra::util
