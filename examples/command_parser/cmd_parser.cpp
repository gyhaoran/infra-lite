#include "command.h"
#include "infra/parsing/result.h"
#include "infra/util/char_stream.h"
#include "infra/parsing/primitives.h"
#include "infra/parsing/combinators.h"
#include <cctype>

using namespace infra::parsing;
using infra::util::skip_ws;

ParseResult<std::string> parse_ident(const char* s) {
    s = skip_ws(s);

    const char* p = s;
    if (!std::isalpha(*p) && *p != '_') {
        return ParseResult<std::string>::error_at(p, ParseError::UnexpectedChar);
    }

    while (std::isalnum(*p) || *p == '_') {
        ++p;
    }

    return { std::string(s, p), p, ParseError::None };
}

ParseResult<int> parse_int(const char* s) {
    s = skip_ws(s);

    int value = 0;
    const char* p = s;

    if (!std::isdigit(*p)) {
        return ParseResult<int>::error_at(p, ParseError::UnexpectedChar);
    }

    while (std::isdigit(*p)) {
        value = value * 10 + (*p - '0');
        ++p;
    }

    return { value, p, ParseError::None };
}

ParseResult<Arg> parse_flag(const char* s) {
    s = skip_ws(s);

    auto dash1 = expect_char(s, '-');
    if (!dash1.ok()) return ParseResult<Arg>::error_at(dash1.next, dash1.error);

    auto dash2 = expect_char(dash1.next, '-');
    if (!dash2.ok()) return ParseResult<Arg>::error_at(dash2.next, dash2.error);

    auto name = parse_ident(dash2.next);
    if (!name.ok()) return ParseResult<Arg>::error_at(name.next, name.error);

    Arg arg;
    arg.kind = Arg::Kind::Flag;
    arg.key = name.value;

    return { arg, name.next, ParseError::None };
}

ParseResult<Arg> parse_kv(const char* s) {
    s = skip_ws(s);

    auto name = parse_ident(s);
    if (!name.ok()) return ParseResult<Arg>::error_at(name.next, name.error);

    const char* p = skip_ws(name.next);
    auto eq = expect_char(p, '=');
    if (!eq.ok()) return ParseResult<Arg>::error_at(eq.next, eq.error);

    auto int_val = parse_int(eq.next);
    if (!int_val.ok()) return ParseResult<Arg>::error_at(int_val.next, int_val.error);

    Arg arg;
    arg.kind = Arg::Kind::KeyValue;
    arg.key = name.value;
    arg.int_value = int_val.value;

    return { arg, int_val.next, ParseError::None };
}

ParseResult<Arg> parse_ident_arg(const char* s) {
    auto id = parse_ident(s);
    if (!id.ok()) return ParseResult<Arg>::error_at(id.next, id.error);

    Arg arg;
    arg.kind = Arg::Kind::Ident;
    arg.key = id.value;

    return { arg, id.next, ParseError::None };
}

ParseResult<Arg> parse_arg(const char* s) {
    s = skip_ws(s);

    // try flag
    auto flag = parse_flag(s);
    if (flag.ok()) {
        return flag;
    }

    // try key-value
    auto kv = parse_kv(s);
    if (kv.ok()) {
        return kv;
    }

    // try identifier
    auto ident = parse_ident_arg(s);
    if (ident.ok()) {
        return ident;
    }

    return ParseResult<Arg>::error_at(s, ParseError::UnexpectedChar);
}

ParseResult<Command> parse_command(const char* s) {
    auto name = parse_ident(s);
    if (!name.ok()) return ParseResult<Command>::error_at(name.next, name.error);

    Command cmd;
    cmd.name = name.value;

    const char* p = name.next;

    while (p && *p != '\0') {
        auto arg = optional(p, parse_arg);
        if (!arg.ok()) {
            break;
        }

        cmd.args.push_back(arg.value);
        p = arg.next;
    }

    return { cmd, p, ParseError::None };
}
