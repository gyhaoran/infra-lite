# infra-lite — Supported Features

本文档用于说明 **infra-lite** 当前版本已经稳定支持的核心能力。  
API 细节、设计原理及扩展用法将在后续版本中逐步补充。

---

## 1. Lightweight Parsing Infrastructure

infra-lite 提供一套**极简、无依赖、零动态分配**的解析基础设施，适用于嵌入式工具、配置解析、小型 DSL 等场景。

- 基于 `const char*` 的输入模型
- 不依赖 STL 容器或异常机制
- 可用于 header-only 方式集成

---

## 2. Parse Result & Error Model

提供统一的解析结果与错误表达模型：

- 明确区分 **成功 / 失败**
- 错误位置通过指针精确定位
- 不隐式跳过输入、不做尾部校验假设

已支持的错误类型：
- `None`
- `UnexpectedChar`
- `UnexpectedEnd`

---

## 3. Parser Function Contract

支持以函数指针形式定义 Parser，并遵循统一契约：

- 成功时返回解析值与未消费的输入位置
- 失败时返回错误类型及错误位置
- Parser 本身不负责跳过多余输入或做全局校验

该设计便于：
- 手写 Parser
- Parser 组合
- 静态分析与调试

---

## 4. Left-Associative Operator Chaining

已支持**左结合运算符链式解析**能力：

- 适用于 `a - b - c`、`a + b + c` 等表达式
- 运算符集合可配置
- 运算逻辑通过回调函数注入

特性：
- 明确的解析顺序
- 不引入 AST 或额外中间结构
- 解析与求值同步完成

---

## 5. Right-Associative Operator Chaining

已支持**右结合运算符链式解析**能力：

- 适用于指数、赋值等右结合语义
- 使用递归方式构建结合关系
- 运算符集合可配置

特性：
- 行为可预测
- 不依赖堆内存
- 与左结合模型风格一致

---

## 6. Whitespace Handling Utilities

提供基础空白字符处理能力：

- 支持空格、制表符、换行符
- 明确由调用方控制何处跳过空白
- 不在 Parser 内部隐式处理，避免语义歧义

---

## 7. Minimal & Composable Design

整体设计遵循以下原则：

- **Minimal**：只提供必要抽象
- **Composable**：Parser 可自由组合
- **Predictable**：无隐藏行为
- **Low-level friendly**：贴近 C 风格，适合系统级代码

---

## Roadmap

后续版本将逐步补充：

- 更丰富的错误类型
- 高阶 Parser 组合工具
- 示例与使用模式文档
- 完整表达式解析示例

（功能列表将随版本持续更新）
