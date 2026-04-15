# infra-lite — Supported Features

本文档描述 **infra-lite** 当前版本已稳定支持的核心能力。

---

## Error Model

- `infra::Error` — 错误类型，支持 None, UnexpectedChar, UnexpectedEnd, Custom
- `Result<T>` — Result 类型，支持 map/and_then

---

## Parser Combinators

### Repetition
- `many(s, p)` — 匹配零个或多个（输入，解析器）
- `many1(s, p)` — 匹配一个或多个

### Choice
- `choice(s, p1, p2)` — 返回第一个成功
- `choice3(s, p1, p2, p3)` — 尝试三个解析器

### Sequence
- `sequence2(s, p1, p2)` — 顺序执行，返回 pair
- `then(s, p, f)` — 解析后转换结果

### Utility
- `optional(s, p)` — 失败时返回默认值
- `lookahead(s, p)` — 不消费输入
- `skip(s, p)` — 丢弃结果
- `sep_by(s, item, sep)` — 解析分隔列表
- `between(s, open, content, close)` — 解析包围内容

### Factory Functions
- `char_p(c)` — 创建匹配精确字符的解析器
- `char_if(pred)` — 创建满足谓词的解析器
- `one_of(chars)` — 创建匹配集合中字符的解析器
- `none_of(chars)` — 创建匹配集合外字符的解析器

---

## String Utilities

- `ltrim(s)` — 跳过前导空白
- `rtrim(s)` — 移除尾随空白
- `starts_with/ends_with` — 前缀/后缀检查
- `find_char(s, c)` — 查找字符
- `skip_word(s)` — 跳过单词
- `strcmp_view(a, b)` — 字符串比较
- `strcasecmp_view(a, b)` — 不区分大小写比较

---

## Memory Utilities

### span<T>
- 非拥有内存视图，零分配
- `data()`, `size()`, `begin()`, `end()`
- `subspan()`, `first()`, `last()`

---

## Logging

- 编译时开关 `INFRA_LOG_ENABLED`
- 零开销（未启用时为空操作）
- `INFRA_LOG_INFO()`, `INFRA_LOG_WARN()`, `INFRA_LOG_ERROR()`

---

## Testing

所有模块都有对应的单元测试：

- `test_calculator` — 计算器示例
- `test_combinators` — 组合器测试
- `test_string_utils` — 字符串工具测试
- `test_error` — 错误模块测试
- `test_span` — span 测试
- `test_logging` — 日志模块测试

运行：`ctest`

## Performance

性能基准测试（`examples/benchmark`）显示所有操作都在亚微秒级：

| 模块 | 操作 | 性能 |
|------|------|------|
| Parsing | char_p | ~0.01 us/op |
| Parsing | many/many1 | ~0.7 us/op |
| String | ltrim/starts_with | ~0.01 us/op |
| Span | iteration/subspan | ~0.01 us/op |

关键特性：
- 零堆分配
- 零外部依赖
- C++17 兼容

---

See [docs/positioning.md](docs/positioning.md) for project scope.