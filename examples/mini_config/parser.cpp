#include "parser.h"

#include "infra/parsing/combinators.h"
#include "infra/util/char_stream.h"

#include <cctype>

using namespace infra::parsing;
using infra::util::skip_ws;

ParseResult<std::string> parse_ident(const char* s) {
    s = skip_ws(s);

    const char* p = s;
    if (!std::isalpha(*p)) {
        return ParseResult<std::string>::error_at(p, ParseError::UnexpectedChar);
    }

    while (std::isalnum(*p) || *p == '_') {
        ++p;
    }

    return { std::string(s, p), p, ParseError::None };
}

ParseResult<int> parse_number(const char* s) {
    s = skip_ws(s);

    int v = 0;
    int len = 0;
    if (sscanf(s, "%d%n", &v, &len) != 1) {
        return ParseResult<int>::error_at(s, ParseError::UnexpectedChar);
    }

    return { v, s + len, ParseError::None };
}

ParseResult<ConfigItem> parse_stmt(const char* s) {
    auto key = parse_ident(s);
    if (!key.ok()) {
        return ParseResult<ConfigItem>::error_at(key.next, key.error);
    }

    auto eq = optional<char>(key.next, [](const char* p) {
        p = skip_ws(p);
        if (*p == '=') {
            return ParseResult<char>{ '=', p + 1, ParseError::None };
        }
        return ParseResult<char>::error_at(p, ParseError::UnexpectedChar);
    });

    ConfigItem item;
    item.key = key.value;

    if (eq.next != key.next) {
        auto val = parse_number(eq.next);
        if (!val.ok()) {
            return ParseResult<ConfigItem>::error_at(val.next, val.error);
        }

        item.has_value = true;
        item.value = val.value;
        return { item, val.next, ParseError::None };
    }

    return { item, key.next, ParseError::None };
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

