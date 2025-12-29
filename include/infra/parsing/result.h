#pragma once

namespace infra::parsing {

enum class ParseError {
    None,
    UnexpectedChar,
    UnexpectedEnd,
};

template<typename T>
struct ParseResult {
    T value{};
    const char* next = nullptr;
    ParseError error = ParseError::None;

    bool ok() const { return error == ParseError::None; }

    static ParseResult error_at(const char* p, ParseError e) {
        ParseResult r;
        r.next = p;
        r.error = e;
        return r;
    }
};

} // namespace infra::parsing
