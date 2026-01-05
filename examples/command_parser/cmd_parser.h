#pragma once

#include "command.h"
#include "infra/parsing/result.h"

infra::parsing::ParseResult<Command> parse_command(const char* s);
