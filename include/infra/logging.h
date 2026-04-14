#pragma once

#include "infra/error.h"
#include <cstdio>

namespace infra::logging {

// ============================================================================
// Simple Logging Module
// ============================================================================
//
// Design principles:
// - Compile-time on/off switch (INFRA_LOG_ENABLED)
// - Zero overhead when disabled
// - Simple API, no external dependencies
//
// Usage:
//   INFRA_LOG_INFO("Hello {}", "world");
//   INFRA_LOG_ERROR("Failed with code {}", 42);
//
// Enable logging by defining INFRA_LOG_ENABLED before including.
// ============================================================================

#ifdef INFRA_LOG_ENABLED

enum class Level {
    Debug,
    Info,
    Warning,
    Error,
};

namespace detail {
    inline void log_message(Level level, const char* file, int line, const char* msg) {
        const char* level_str = "DEBUG";
        if (level == Level::Info) level_str = "INFO ";
        else if (level == Level::Warning) level_str = "WARN ";
        else if (level == Level::Error) level_str = "ERROR";

        std::fprintf(stderr, "[%s] %s:%d: %s\n", level_str, file, line, msg);
    }
}  // namespace detail

#define INFRA_LOG_DEBUG(msg) infra::logging::detail::log_message(infra::logging::Level::Debug, __FILE__, __LINE__, msg)
#define INFRA_LOG_INFO(msg) infra::logging::detail::log_message(infra::logging::Level::Info, __FILE__, __LINE__, msg)
#define INFRA_LOG_WARN(msg) infra::logging::detail::log_message(infra::logging::Level::Warning, __FILE__, __LINE__, msg)
#define INFRA_LOG_ERROR(msg) infra::logging::detail::log_message(infra::logging::Level::Error, __FILE__, __LINE__, msg)

#else

// Logging disabled - no-op macros
#define INFRA_LOG_DEBUG(msg) ((void)0)
#define INFRA_LOG_INFO(msg) ((void)0)
#define INFRA_LOG_WARN(msg) ((void)0)
#define INFRA_LOG_ERROR(msg) ((void)0)

#endif  // INFRA_LOG_ENABLED

}  // namespace infra::logging