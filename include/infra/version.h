#pragma once

/// @file
/// @brief infra-lite version information

namespace infra {

/// @brief Major version number (breaking changes)
constexpr int VERSION_MAJOR = 0;

/// @brief Minor version number (new features, backward compatible)
constexpr int VERSION_MINOR = 1;

/// @brief Patch version number (bug fixes)
constexpr int VERSION_PATCH = 0;

/// @brief Version string (e.g., "0.1.0")
constexpr const char* VERSION_STRING = "0.1.0";

/// @brief Version tuple for programmatic access
#define INFRA_VERSION ((VERSION_MAJOR << 16) | (VERSION_MINOR << 8) | VERSION_PATCH)

} // namespace infra
