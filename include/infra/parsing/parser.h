// Parser contract:
// - On success: error == None, next points to first unconsumed char
// - On failure: error != None, next points to error position
// - Parser MUST NOT skip trailing input validation

#pragma once
#include "result.h"

namespace infra::parsing {

template<typename T>
using Parser = ParseResult<T>(*)(const char*);

} // namespace infra::parsing
