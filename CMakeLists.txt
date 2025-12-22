
cmake_minimum_required(VERSION 3.10)
project(infra_lite LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_library(infra-lite INTERFACE)
target_include_directories(infra-lite INTERFACE include)

file(GLOB_RECURSE ExampleCalculatorSources examples/calculator/*.cpp)

add_executable(calculator ${ExampleCalculatorSources})
target_link_libraries(calculator infra-lite)

enable_testing()

add_executable(test_calculator
    tests/test_calculator.cpp
    examples/calculator/calculator.cpp
)

target_include_directories(test_calculator PRIVATE
    tests
    include
)

add_test(NAME calculator_tests COMMAND test_calculator)
