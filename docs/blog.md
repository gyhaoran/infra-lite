# 我用 100 行 C 代码，写了一个“新手也能看懂”的解析框架

很多程序员在工作 2~3 年后，都会遇到一个坎：

> 看得懂业务代码
但一看到 **parser** / **compiler** / **infra** 就本能地想躲

我自己也经历过这个阶段。
这篇文章，我想用一个**极小的算术计算器**，带你理解：

- parser 到底在做什么
- 为什么很多“复杂设计”其实是可以避免的
- 什么样的代码，才配得上被叫做 **infra 级代码**

不讲编译原理，不画 AST。
只讲：**人是怎么一步步把它想出来的。**

---

## 一、我们真的需要 AST 吗？

假设你要支持这样一个表达式：
```text
1 + 2 * (3 + 4) - 5
```
很多教程会告诉你：
1. 写 tokenizer
2. 构建 AST
3. visitor 遍历
4. evaluate

问题是：
- 代码多
- 状态复杂
- 对小工具 / infra 来说严重过度设计

但如果你只是想要一个结果：
```c
int eval(const char* expr);
```
那 AST 并不是必须的。

---

## 二、表达式本身就是递归的

我们先从最终的入口函数开始：
```c
int eval(const char* expr) {
    return parse_add_sub(expr).value;
}
```

这里已经隐含了一件事：
> 加减是最高层语法结构
而加减的“子结构”，是乘除。

---

## 三、消灭全局游标：ParseResult

很多初学者会这样写：
```c
const char* g_cur;
```

所有解析函数都偷偷用它。
这是个非常糟糕的习惯。
我们换一种方式：
```c
struct ParseResult {
    int value;
    const char* next;
};
```
含义非常直观：
- value：解析出来的值
- next：还没被消费的字符位置
于是，每个 parser 都是一个**纯函数**：
```c
ParseResult parse_xxx(const char* input);
```

---

## 四、最底层：解析数字和括号

最简单的 parser 是“数字”：
```c
ParseResult parse_number(const char* s) {
    int value = 0;
    while (*s >= '0' && *s <= '9') {
        value = value * 10 + (*s - '0');
        ++s;
    }
    return { value, s };
}
```

而括号，本质上是“递归回到最高优先级”：
```c
ParseResult parse_group(const char* s) {
    if (*s == '(') {
        auto r = parse_add_sub(s + 1);
        return { r.value, r.next + 1 }; // skip ')'
    }
    return parse_number(s);
}
```
到这里为止，一切都非常直觉。

## 五、一元运算：负号是“递归叠加”的

负号其实比看起来简单：
```c
ParseResult parse_negate(const char* s) {
    if (*s == '-') {
        auto r = parse_negate(s + 1);
        return { -r.value, r.next };
    }
    return parse_group(s);
}
```
重点在于这一点：
> **一元运算天然就是递归的**

`---1`会自然地被解析。

## 六、真正的核心：chain 抽象

现在观察加减、乘除：
```text
1 + 2 + 3
4 * 5 / 2
```
它们本质上是同**一种模式**：
- 左结合
- 同一优先级
- 可重复
于是我们抽象出一个通用函数：
```c
using Parser = ParseResult (*)(const char*);

ParseResult chain(
    const char* s,
    Parser sub,
    char op1,
    char op2
) {
    auto left = sub(s);

    while (*left.next == op1 || *left.next == op2) {
        char op = *left.next;
        auto right = sub(left.next + 1);

        if (op == op1) { 
            left.value += right.value; 
        } else {
             left.value -= right.value;
        }
        left.next = right.next;
    }

    return left;
}
```
这是整个框架最重要的一段代码。

## 七、用 chain 组合出完整语法

有了 chain，语法层级就变得非常清晰：
```c
ParseResult parse_mul_div(const char* s) {
    return chain(s, parse_negate, '*', '/');
}

ParseResult parse_add_sub(const char* s) {
    return chain(s, parse_mul_div, '+', '-');
}
```
**注意顺序**：
- add_sub → mul_div → negate → group → number

这正是运算符优先级。

## 八、为什么这套设计适合 infra？

因为它满足了 infra 的核心需求：
- 无全局状态（可重入）
- 无隐藏副作用
- 易测试
- 易扩展
- 易长期维护

你几年后回来看，仍然能看懂。

## 九、如何扩展？

**新运算符？**
- 新增一个 parser
- 用 chain 组合
- 插入优先级层级

**新语法？**
- 在 group() 中扩展
- 不影响其它层级

这就是 可演进的设计。

## 十、你真正该学到的不是“计算器”

而是这些思维方式：
- 用递归结构表达语法
- 显式传递状态
- 把重复模式抽象成函数
- 控制复杂度，而不是堆功能

这是从“能写代码”，走向“能写长期系统”的一步。

## 写在最后
这个 framework 并不是为了炫技。

它的目标只有一个：

> **让你在几年后回看代码时，依然能看懂**

如果你能完全理解这套设计，
说明你已经开始具备 **基础设施工程师的思维方式**。
