#pragma once

#include "config.h"
#include "infra/parsing/result.h"

infra::parsing::ParseResult<ConfigItem> parse_stmt(const char* s);
infra::parsing::ParseResult<Config> parse_config(const char* s);
