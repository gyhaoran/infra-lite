# infra-lite

**一个轻量级、可嵌入的 C / C++ 基础设施（Infrastructure）代码集合。**

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)

---

## 什么是 infra-lite？

infra-lite 是一组 **小而精、可复用** 的基础设施组件，适用于那些：

- 经常被重复实现
- 但又不值得引入大型框架（如 Abseil、Folly）

> "先有基础设施，再有功能。"

**核心定位**：显式、可组合、零依赖的 parsing 原语和错误处理工具。

---

## 设计理念

| 原则 | 含义 |
|------|------|
| **基础设施优先于功能** | 先打好地基 |
| **简单优先于完整** | 做少做精 |
| **显式优先于隐式** | 无隐藏状态，无魔法 |
| **组合优先于整体** | 小块拼装，大有可为 |

---

## 核心特性

- **Header-first** — 无编译开销，头文件即用
- **零依赖** — 纯 C++17，无外部库
- **可嵌入** — 适用于嵌入式、EDA 等受限环境
- **显式** — 无隐藏状态，无魔法
- **可组合** — 小原语，灵活拼接

---

## 非目标（infra-lite 不是什么）

infra-lite 有意克制，明确不是：

1. ❌ 容器库（不做 STL 替代品）
2. ❌ 异步框架（不做大规模并发）
3. ❌ 反射系统
4. ❌ "全家桶"工具库
5. ❌ 基于异常的错误处理（使用显式返回值）
6. ❌ STL 依赖的核心抽象

如需更大型的能力，请考虑 [Abseil](https://abseil.io/)、[Folly](https://github.com/facebook/folly) 或 [Boost](https://www.boost.org/)。

---

## 当前支持的能力

### Parsing 基础设施
- 统一的 result/error 模型
- Primitive parsers（字符匹配、谓词匹配）
- Parser combinators（左/右结合链式组合）
- 显式空白字符处理

### 设计原则
- 无隐式内存分配
- 无隐藏状态
- 控制流可预测、可追踪

详见 [docs/features.md](docs/features.md)。

---

## 快速上手

```cpp
#include "infra/parsing/parser.h"
#include "infra/parsing/combinators.h"

using namespace infra::parsing;

// 简单 parser：匹配一个或多个数字
auto number = many1(char_p(isdigit));

// 使用
const char* input = "123abc";
auto result = number(input);
if (result) {
    // result.value() 包含 "123"
    // result.end() 指向 "abc"
}
```

---

## 构建与测试

```bash
# 克隆并构建
mkdir build && cd build
cmake ..
cmake --build .

# 运行测试
ctest

# 运行示例
./examples/calculator
./examples/cmd_parser
```

---

## 项目结构

```
include/
├── infra/
│   ├── error.h          # （规划中）统一错误模型
│   ├── parsing/
│   │   ├── parser.h     # Parser 契约
│   │   ├── primitives.h # 基础 parsers
│   │   ├── combinators.h# Combinators
│   │   ├── chain.h      # 运算符链式组合
│   │   └── result.h     # Parse 结果
│   └── util/
│       └── char_stream.h# 输入工具
docs/
├── features.md          # 当前能力清单
├── positioning.md        # 项目定位
├── roadmap.md            # 开发计划
└── parsing_framework_guide.md
examples/
├── calculator.cpp       # 表达式求值器
└── cmd_parser.cpp       # CLI 参数解析器
tests/
└── ...
```

---

## 文档

- [定位说明](docs/positioning.md) — 项目边界和非目标
- [功能清单](docs/features.md) — 已支持的能力
- [路线图](docs/roadmap.md) — 开发计划（8-12 周）
- [Parsing 指南](docs/parsing_framework_guide.md) — 设计理念

---

## 贡献指南

详见 [CONTRIBUTING.md](CONTRIBUTING.md)：
- 代码风格规范
- 提交约定
- 设计原则（SOLID、RAII、const-correctness）

---

## License

MIT License. 见 [LICENSE](LICENSE)。