// =============================================================================
// Parser Contract
// =============================================================================
// 
// A Parser<T> is a function: const char* -> ParseResult<T>
// 
// Contract:
//   - On success: ok() == true, value contains parsed result
//                 next points to first unconsumed character
//   - On failure: ok() == false, error contains failure reason
//                 next points to error position
//
// Rules:
//   1. Parser MUST NOT skip trailing input validation
//   2. On failure, next MUST point to error position (not consumed input)
//   3. No hidden state - same input always produces same output
//   4. No exceptions - errors are returned via ParseResult
//
// Design Principles:
//   - Infrastructure over features (foundation before convenience)
//   - Explicit over implicit (no hidden state)
//   - Composable over monolithic (small pieces that fit together)
//
// =============================================================================

#pragma once

#include "result.h"

namespace infra::parsing {

/// Parser<T> - Type alias for parser functions.
/// 
/// A parser is a function that takes an input string and returns:
///   - On success: parsed value + position of first unconsumed char
///   - On failure: error + position where error occurred
template<typename T>
using Parser = ParseResult<T>(*)(const char*);

} // namespace infra::parsing