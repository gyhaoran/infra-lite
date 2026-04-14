cmake_minimum_required(VERSION 3.10)
project(infra_lite LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_library(infra-lite INTERFACE)
target_include_directories(infra-lite INTERFACE include)

file(GLOB_RECURSE ExampleCalculatorSources examples/calculator/*.cpp)
file(GLOB_RECURSE ExampleMiniConfigSources examples/mini_config/*.cpp)
file(GLOB_RECURSE ExampleCommandParserSources examples/command_parser/*.cpp)
file(GLOB_RECURSE ExampleErrorSources examples/error_example/*.cpp)

add_executable(calculator ${ExampleCalculatorSources})
target_link_libraries(calculator infra-lite)

add_executable(mini_config ${ExampleMiniConfigSources})
target_link_libraries(mini_config infra-lite)

add_executable(command_parser ${ExampleCommandParserSources})
target_link_libraries(command_parser infra-lite)

add_executable(error_example ${ExampleErrorSources})
target_link_libraries(error_example infra-lite)

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

add_executable(test_combinators tests/test_combinators.cpp)
target_link_libraries(test_combinators infra-lite)
add_test(NAME combinator_tests COMMAND test_combinators)

add_executable(test_string_utils tests/test_string_utils.cpp)
target_link_libraries(test_string_utils infra-lite)
add_test(NAME string_utils_tests COMMAND test_string_utils)

add_executable(test_error tests/test_error.cpp)
target_link_libraries(test_error infra-lite)
add_test(NAME error_tests COMMAND test_error)

add_executable(test_span tests/test_span.cpp)
target_link_libraries(test_span infra-lite)
add_test(NAME span_tests COMMAND test_span)