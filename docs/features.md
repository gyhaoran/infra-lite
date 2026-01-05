# infra-lite — Supported Features

本文档描述 **infra-lite** 当前版本已稳定支持的核心能力。  
该列表关注的是 **能力边界与设计层级**，而非 API 用法或实现细节。

具体接口说明、设计推导及示例将随版本逐步补充。

---

## Overview

infra-lite 是一组 **轻量级、可嵌入、可组合** 的 C / C++ 基础设施组件，  
面向以下使用场景：

- 配置解析
- 表达式求值
- 小型 DSL
- 系统级或工具级代码

整体设计强调：

- 行为显式
- 控制流可预测
- 抽象层级稳定
- 长期可维护性

---

## Parsing Architecture

infra-lite 的 parsing 模块采用 **分层式能力结构**，而非单一解析框架。

从低到高分为：

1. **Result & Error Model**
2. **Primitive Parsers**
3. **Parser Contract**
4. **Parser Combinators**
5. **Usage-level Composition（由用户代码完成）**

infra-lite 只覆盖前四层。

---

## Parse Result & Error Model

infra-lite 使用统一的解析结果与错误表达模型。

### 核心特性

- 明确区分 **成功 / 失败**
- 错误位置通过 `const char*` 精确定位
- 不依赖异常机制
- 不进行隐式错误恢复

### 已定义错误类型

- `None`
- `UnexpectedChar`
- `UnexpectedEnd`

错误模型不对“是否允许继续解析”做假设，由调用方决定处理策略。

---

## Primitive Parsers

Primitive Parser 是 infra-lite parsing 的 **最小不可分解单元**。

它们只关心：

- 当前输入字符
- 是否满足某个条件
- 是否消费输入

它们**不负责**：

- 跳过空白
- 语义解释
- 循环或组合
- 回溯或预测

### 已支持的 Primitive

#### Character Expectation

- 精确匹配指定字符
- 成功时消费一个字符
- 失败时返回明确错误位置

#### Any Character

- 消费任意一个字符
- 不关心字符内容
- 在 EOF 时失败

#### Predicate-based Matching

- 根据用户提供的 predicate 判断字符
- 满足条件时消费输入
- 否则失败

该能力用于构建标识符、数字、字符串等上层解析逻辑。

---

## Parser Contract

infra-lite 中的 Parser 以函数形式定义，并遵循统一契约：

### 成功时

- 返回解析值
- 返回第一个未消费的输入位置

### 失败时

- 返回错误类型
- 返回错误发生位置

Parser 本身 **不承担以下职责**：

- 自动跳过空白
- 进行全局输入校验
- 隐式回溯或状态管理

这一契约使 Parser 易于：

- 手写
- 组合
- 推理
- 调试

---

## Parser Combinators

infra-lite 提供一组 **低阶组合工具**，用于表达常见解析模式。

### Operator Chaining

用于处理表达式中的结合关系。

#### Left-Associative Chaining

适用于：

- 加减
- 乘除
- 其他左结合语义

特性：

- 明确的解析顺序
- 运算逻辑通过回调注入
- 不引入中间 AST

#### Right-Associative Chaining

适用于：

- 幂运算
- 赋值类语义
- 其他右结合结构

特性：

- 使用递归构建结合关系
- 行为可预测
- 与左结合模型保持一致风格

---

## Input Control Utilities

infra-lite 提供一组用于 **精确控制输入消费行为** 的基础工具。

### Whitespace Handling

- 支持空格、制表符、换行符
- 由调用方显式决定跳过位置
- 不在 Parser 内部隐式处理

该设计避免语义歧义，并保持控制流透明。

---

## Composition & Predictability

infra-lite 强调：

- 所有输入消费都是显式的
- Parser 行为无隐藏状态
- 组合后的行为仍然可推理

解析流程应当：

- 易于阅读
- 易于调试
- 行为稳定、无意外副作用

---

## Design Constraints

infra-lite 明确遵循以下约束：

- 不提供完整解析框架
- 不引入自动错误恢复机制
- 不强制构建 AST
- 不追求语法覆盖率最大化
- 不依赖 STL 容器或异常

infra-lite 的目标是提供  
**稳定、低阶、可组合的解析原语**，  
而不是一站式解析解决方案。

---

## Roadmap

未来版本可能逐步补充：

- 更细粒度的错误分类
- 额外的低阶组合原语
- 使用模式与设计示例
- 更复杂语法的参考实现

（功能列表将随能力演进持续更新）
