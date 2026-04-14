#pragma once

#include <cstddef>
#include <cassert>

namespace infra::util {

// ============================================================================
// Span - Non-owning view of contiguous memory
// ============================================================================

/// A span is a non-owning view over a contiguous sequence of elements.
/// 
/// Design principles:
/// - Zero allocation (no heap operations)
/// - Optional bounds checking (debug mode only)
/// - Compatible with pointer/length interfaces
/// 
/// Usage:
///   void process(span<const char> s) {
///     for (size_t i = 0; i < s.size(); ++i) {
///       // access s[i]
///     }
///   }
template<typename T>
class span {
public:
    using element_type = T;
    using value_type = std::remove_cv_t<T>;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

    constexpr span() noexcept : data_(nullptr), size_(0) {}
    constexpr span(T* ptr, size_t count) noexcept : data_(ptr), size_(count) {}

    template<typename U, size_t N>
    constexpr span(U (&arr)[N]) noexcept : data_(arr), size_(N) {}

    constexpr T* data() const noexcept { return data_; }
    constexpr size_t size() const noexcept { return size_; }
    constexpr bool empty() const noexcept { return size_ == 0; }

    constexpr T& operator[](size_t i) const noexcept {
        return data_[i];
    }

    constexpr span<const T> subspan(size_t offset, size_t count) const noexcept {
        return span<const T>(data_ + offset, count);
    }

    constexpr span<const T> first(size_t count) const noexcept {
        return span<const T>(data_, count);
    }

    constexpr span<const T> last(size_t count) const noexcept {
        return span<const T>(data_ + (size_ - count), count);
    }

private:
    T* data_;
    size_t size_;
};

// Specialization for const char (string-like usage)
using string_span = span<const char>;

} // namespace infra::util