#pragma once

#include <cstring>
#include <cctype>

namespace infra::util {

// ============================================================================
// String View Utilities (zero-allocation)
// ============================================================================

/// Trim leading whitespace.
/// @return Pointer to first non-whitespace char, or end of string.
inline const char* ltrim(const char* s) {
    while (*s && (*s == ' ' || *s == '\t' || *s == '\n' || *s == '\r')) {
        ++s;
    }
    return s;
}

/// Trim trailing whitespace (modifies string).
/// @return Pointer to the new end (null terminator).
inline char* rtrim(char* s) {
    char* end = const_cast<char*>(s) + std::strlen(s);
    while (end > s && (end[-1] == ' ' || end[-1] == '\t' || end[-1] == '\n' || end[-1] == '\r')) {
        --end;
    }
    *end = '\0';
    return end;
}

/// Trim both leading and trailing whitespace.
/// @return Pointer to trimmed string.
inline const char* trim(const char* s) {
    return ltrim(s);
}

/// Check if string starts with prefix.
inline bool starts_with(const char* s, const char* prefix) {
    while (*prefix) {
        if (*s != *prefix) {
            return false;
        }
        ++s;
        ++prefix;
    }
    return true;
}

/// Check if string ends with suffix.
inline bool ends_with(const char* s, const char* suffix) {
    size_t s_len = std::strlen(s);
    size_t suf_len = std::strlen(suffix);
    if (s_len < suf_len) {
        return false;
    }
    return std::strcmp(s + s_len - suf_len, suffix) == 0;
}

/// Find character in string, return pointer or nullptr.
inline const char* find_char(const char* s, char c) {
    while (*s) {
        if (*s == c) {
            return s;
        }
        ++s;
    }
    return nullptr;
}

/// Advance past word characters (alphanumeric + underscore).
inline const char* skip_word(const char* s) {
    while (*s && (std::isalnum(static_cast<unsigned char>(*s)) || *s == '_')) {
        ++s;
    }
    return s;
}

} // namespace infra::util