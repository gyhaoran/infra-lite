// infra-lite — Performance Benchmark
// Simple zero-dependency benchmarks demonstrating infra-lite characteristics
//
// Build: cmake --build . --target benchmark
// Run: ./benchmark

#include <chrono>
#include <cstring>
#include <cstdio>
#include <cctype>

#include "infra/parsing/parser.h"
#include "infra/parsing/primitives.h"
#include "infra/parsing/combinators.h"
#include "infra/util/string_utils.h"
#include "infra/util/span.h"

// ============================================================================
// Simple Benchmark Framework (zero-dependency)
// ============================================================================

class Benchmark {
public:
    using Func = void(*)();

    Benchmark(const char* name, Func func, int iterations = 100000)
        : name_(name), func_(func), iterations_(iterations) {}

    void run() {
        // Warmup
        for (int i = 0; i < 100; ++i) {
            func_();
        }

        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < iterations_; ++i) {
            func_();
        }
        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        double per_iter = static_cast<double>(duration.count()) / iterations_;
        
        printf("%-35s %10.2f us/op  (%d iterations)\n", name_, per_iter, iterations_);
    }

private:
    const char* name_;
    Func func_;
    int iterations_;
};

#define RUN_BENCHMARK(name, func, iter) \
    do { \
        printf("\n%s:\n", name); \
        Benchmark(#func, func, iter).run(); \
    } while(0)

// ============================================================================
// Test Data
// ============================================================================

const char* g_input1 = "123456";
const char* g_input2 = "abc";
const char* g_input3 = "xyz";
const char* g_input4 = "abc";
const char* g_input5 = "   hello world";
const char* g_input6 = "prefix_value";
const char* g_input7 = "hello_world_test";
int g_digits[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

// ============================================================================
// Parsing Benchmarks — Zero Allocation
// ============================================================================

using namespace infra::parsing;

// Parser: match digit (free function style)
infra::parsing::ParseResult<char> parse_digit(const char* s) {
    if (*s == '\0') {
        return infra::parsing::ParseResult<char>::unexpected_end(s);
    }
    if (!std::isdigit(static_cast<unsigned char>(*s))) {
        return infra::parsing::ParseResult<char>::unexpected_char(s);
    }
    return infra::parsing::ParseResult<char>::success(*s, s + 1);
}

// Parser: match 'a'
infra::parsing::ParseResult<char> parse_a(const char* s) {
    if (*s == '\0') {
        return infra::parsing::ParseResult<char>::unexpected_end(s);
    }
    if (*s != 'a') {
        return infra::parsing::ParseResult<char>::unexpected_char(s);
    }
    return infra::parsing::ParseResult<char>::success(*s, s + 1);
}

// Parser: match 'b'
infra::parsing::ParseResult<char> parse_b(const char* s) {
    if (*s == '\0') {
        return infra::parsing::ParseResult<char>::unexpected_end(s);
    }
    if (*s != 'b') {
        return infra::parsing::ParseResult<char>::unexpected_char(s);
    }
    return infra::parsing::ParseResult<char>::success(*s, s + 1);
}

// Parser: match 'x'
infra::parsing::ParseResult<char> parse_x(const char* s) {
    if (*s == '\0') {
        return infra::parsing::ParseResult<char>::unexpected_end(s);
    }
    if (*s != 'x') {
        return infra::parsing::ParseResult<char>::unexpected_char(s);
    }
    return infra::parsing::ParseResult<char>::success(*s, s + 1);
}

// Parser: match whitespace
infra::parsing::ParseResult<char> parse_space(const char* s) {
    if (*s == '\0') {
        return infra::parsing::ParseResult<char>::unexpected_end(s);
    }
    if (*s != ' ' && *s != '\t') {
        return infra::parsing::ParseResult<char>::unexpected_char(s);
    }
    return infra::parsing::ParseResult<char>::success(*s, s + 1);
}

// Parser: match alphanumeric
infra::parsing::ParseResult<char> parse_alnum(const char* s) {
    if (*s == '\0') {
        return infra::parsing::ParseResult<char>::unexpected_end(s);
    }
    if (!std::isalnum(static_cast<unsigned char>(*s)) && *s != '_') {
        return infra::parsing::ParseResult<char>::unexpected_char(s);
    }
    return infra::parsing::ParseResult<char>::success(*s, s + 1);
}

// Helper: parse optional spaces
infra::parsing::ParseResult<char> parse_opt_space(const char* s) {
    return optional(s, parse_space);
}

void BM_CharParser() {
    char_p('1')(g_input1);
}

void BM_DigitSequence() {
    many(g_input1, parse_digit);
}

void BM_DigitSequence1() {
    many1(g_input1, parse_digit);
}

void BM_ChoiceCombinator() {
    choice(g_input3, parse_a, parse_b);
}

void BM_SequenceCombinator() {
    sequence2(g_input4, parse_a, parse_b);
}

void BM_OptionalParser() {
    parse_opt_space(g_input5);
}

void BM_SkipParser() {
    skip(g_input5, parse_space);
}

void BM_AnyChar() {
    any_char(g_input1);
}

void BM_Satisfy() {
    satisfy(g_input1, ::isdigit);
}

void BM_OneOf() {
    one_of("+-*/")(g_input1);
}

void BM_NoneOf() {
    none_of(" \t\n")(g_input1);
}

// ============================================================================
// String Utilities Benchmarks —Zero Allocation
// ============================================================================

void BM_LTrim() {
    infra::util::ltrim(g_input5);
}

void BM_RTrim() {
    char s[] = "hello world   ";
    infra::util::rtrim(s);
}

void BM_StartsWith() {
    infra::util::starts_with(g_input6, "prefix");
}

void BM_EndsWith() {
    infra::util::ends_with(g_input6, "value");
}

void BM_FindChar() {
    infra::util::find_char(g_input7, '_');
}

void BM_StrcmpView() {
    infra::util::strcmp_view(g_input7, "hello_world_test");
}

// ============================================================================
// Span Benchmarks — Zero Allocation
// ============================================================================

void BM_SpanIteration() {
    infra::util::span<int> s(g_digits, 10);
    int sum = 0;
    for (auto elem : s) {
        sum += elem;
    }
}

void BM_SpanSubview() {
    infra::util::span<int> s(g_digits, 10);
    auto sub = s.subspan(2, 4);
}

void BM_SpanFirst() {
    infra::util::span<int> s(g_digits, 10);
    auto first = s.first(5);
}

void BM_SpanLast() {
    infra::util::span<int> s(g_digits, 10);
    auto last = s.last(5);
}

// ============================================================================
// Main
// ============================================================================

int main() {
    printf("========================================\n");
    printf("infra-lite Performance Benchmark\n");
    printf("Zero-dependency, C++17, Stack-only\n");
    printf("========================================\n");

    // =========================================================================
    printf("\n[ Parsing Benchmarks ]\n");
    printf("%-35s %s\n", "Test", "Time/op");
    printf("--------------------------------------\n");
    
    RUN_BENCHMARK("Single char match", BM_CharParser, 1000000);
    RUN_BENCHMARK("Digit sequence (many, 0+)", BM_DigitSequence, 500000);
    RUN_BENCHMARK("Digit sequence (many1, 1+)", BM_DigitSequence1, 500000);
    RUN_BENCHMARK("Choice (2 alternatives)", BM_ChoiceCombinator, 500000);
    RUN_BENCHMARK("Sequence (2 parsers)", BM_SequenceCombinator, 500000);
    RUN_BENCHMARK("Optional parser", BM_OptionalParser, 500000);
    RUN_BENCHMARK("Skip parser", BM_SkipParser, 500000);
    RUN_BENCHMARK("Any char", BM_AnyChar, 1000000);
    RUN_BENCHMARK("Satisfy predicate", BM_Satisfy, 1000000);
    RUN_BENCHMARK("One of chars", BM_OneOf, 1000000);
    RUN_BENCHMARK("None of chars", BM_NoneOf, 1000000);

    // =========================================================================
    printf("\n[ String Utilities Benchmarks ]\n");
    printf("%-35s %s\n", "Test", "Time/op");
    printf("--------------------------------------\n");

    RUN_BENCHMARK("Left trim", BM_LTrim, 1000000);
    RUN_BENCHMARK("Right trim", BM_RTrim, 1000000);
    RUN_BENCHMARK("Starts with", BM_StartsWith, 1000000);
    RUN_BENCHMARK("Ends with", BM_EndsWith, 1000000);
    RUN_BENCHMARK("Find char", BM_FindChar, 1000000);
    RUN_BENCHMARK("String compare view", BM_StrcmpView, 500000);

    // =========================================================================
    printf("\n[ Span Benchmarks ]\n");
    printf("%-35s %s\n", "Test", "Time/op");
    printf("--------------------------------------\n");

    RUN_BENCHMARK("Span iteration (10 elements)", BM_SpanIteration, 500000);
    RUN_BENCHMARK("Span subspan", BM_SpanSubview, 1000000);
    RUN_BENCHMARK("Span first(n)", BM_SpanFirst, 1000000);
    RUN_BENCHMARK("Span last(n)", BM_SpanLast, 1000000);

    // =========================================================================
    printf("\n========================================\n");
    printf("Key Characteristics:\n");
    printf("  - All operations are STACK-ONLY\n");
    printf("  - Zero heap allocations\n");
    printf("  - Zero external dependencies\n");
    printf("  - C++17 compliant\n");
    printf("========================================\n");

    return 0;
}
