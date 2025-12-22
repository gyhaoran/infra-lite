
#pragma once
#include "result.h"

template<typename T>
using Parser = ParseResult<T>(*)(const char*);
