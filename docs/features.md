# infra-lite — Supported Features

本文档描述 **infra-lite** 当前版本已稳定支持的核心能力。

---

## Parser Combinators

### Repetition
- `many(p)` — 匹配零个或多个
- `many1(p)` — 匹配一个或多个

### Choice
- `choice(p1, p2)` — 返回第一个成功
- `choice3(p1, p2, p3)` — 尝试三个解析器

### Sequence
- `sequence2(p1, p2)` — 顺序执行，返回 pair
- `then(p, f)` — 解析后转换结果

### Utility
- `optional(p)` — 失败时返回默认值
- `lookahead(p)` — 不消费输入
- `skip(p)` — 丢弃结果
- `sep_by(item, sep)` — 解析分隔列表
- `between(open, content, close)` — 解析包围内容

### Factory Functions
- `char_p(c)` — 匹配精确字符
- `char_if(pred)` — 满足谓词
- `one_of(chars)` — 匹配集合中字符
- `none_of(chars)` — 匹配集合外字符

---

## String Utilities

- `ltrim(s)` — 跳过前导空白
- `rtrim(s)` — 移除尾随空白
- `starts_with/ends_with` — 前缀/后缀检查
- `find_char(s, c)` — 查找字符
- `skip_word(s)` — 跳过单词

---

## Error Model

使用 `infra::Error` 和 `Result<T>` 提供统一错误处理。

---

See [docs/positioning.md](docs/positioning.md) for project scope.