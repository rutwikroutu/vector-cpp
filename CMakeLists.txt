cmake_minimum_required(VERSION 3.20)
project(dev_vector LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

find_package(GTest REQUIRED)

add_executable(vector_tests
    tests/vector_tests.cpp
)

target_include_directories(vector_tests PRIVATE include)

target_link_libraries(vector_tests
    GTest::gtest
    GTest::gtest_main
)

add_test(NAME vector_tests COMMAND vector_tests)