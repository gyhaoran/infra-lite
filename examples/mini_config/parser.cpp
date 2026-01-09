#include "parser.h"

#include "infra/parsing/primitives.h"
#include "infra/util/char_stream.h"

#include <cctype>

using namespace infra::parsing;
using infra::util::skip_ws;

ParseResult<std::string> parse_ident(const char* s) {
    s = skip_ws(s);
    auto r = satisfy(s, [](char c) { return std::isalpha(c) || c == '_'; });
    if (!r.ok()) { 
        return ParseResult<std::string>::error_at(s, r.error); 
    }

    std::string out;
    const char* p = s;

    while (true) {
        auto c = satisfy(p, [](char c) { return std::isalnum(c) || c == '_'; });
        if (!c.ok()) break;

        out.push_back(c.value);
        p = c.next;
    }

    return { out, p, ParseError::None };
}

ParseResult<int> parse_int(const char* s) {
    s = skip_ws(s);

    int v = 0;
    const char* p = s;

    auto d = satisfy(p, ::isdigit);
    if (!d.ok()) {
        return ParseResult<int>::error_at(s, ParseError::UnexpectedChar);
    }

    while (true) {
        auto r = satisfy(p, ::isdigit);
        if (!r.ok()) break;
        v = v * 10 + (r.value - '0');
        p = r.next;
    }

    return { v, p, ParseError::None };
}


ParseResult<std::string> parse_string(const char* s) {
    s = skip_ws(s);

    auto open = expect_char(s, '"');
    if (!open.ok()) {
        return ParseResult<std::string>::error_at(open.next, open.error);
    }

    std::string out;
    const char* p = open.next;

    while (true) {
        if (*p == '\0') {
            return ParseResult<std::string>::error_at(p, ParseError::UnexpectedEnd);
        }

        if (*p == '"') {
            return { out, p + 1, ParseError::None };
        }

        auto c = any_char(p);
        out.push_back(c.value);
        p = c.next;
    }
}

ParseResult<ConfigItem> parse_value(const char* s, ConfigItem item) {
    auto iv = parse_int(s);
    if (iv.ok()) {
        item.type = ConfigItem::ValueType::Int;
        item.int_value = iv.value;
        return { item, iv.next, ParseError::None };
    }

    auto sv = parse_string(s);
    if (sv.ok()) {
        item.type = ConfigItem::ValueType::String;
        item.str_value = sv.value;
        return { item, sv.next, ParseError::None };
    }

    return ParseResult<ConfigItem>::error_at(s, ParseError::UnexpectedChar);
}

ParseResult<ConfigItem> parse_stmt(const char* s) {
    auto key = parse_ident(s);
    if (!key.ok()) {
        return ParseResult<ConfigItem>::error_at(key.next, key.error);
    }

    ConfigItem item{.key = key.value};
    const char* p = skip_ws(key.next);

    auto eq = expect_char(p, '=');
    if (!eq.ok()) {
        // key-only statement is allowed
        return { item, key.next, ParseError::None };
    }

    return parse_value(eq.next, item);
}

ParseResult<Config> parse_config(const char* s) {
    Config cfg;
    const char* p = s;

    while (true) {
        p = skip_ws(p);
        if (*p == '\0') {
            break;
        }

        auto stmt = parse_stmt(p);
        if (!stmt.ok()) {
            return ParseResult<Config>::error_at(stmt.next, stmt.error);
        }

        cfg.push_back(stmt.value);
        p = stmt.next;
    }

    return { cfg, p, ParseError::None };
}
