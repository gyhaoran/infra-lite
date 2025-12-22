
cmake_minimum_required(VERSION 3.10)
project(infra_lite LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_library(infra-lite INTERFACE)
target_include_directories(infra-lite INTERFACE include)

add_executable(calculator examples/calculator/main.cpp)
target_link_libraries(calculator infra-lite)
