cmake_minimum_required(VERSION 3.0.0)
project(ARCVM-V2 VERSION 0.1.0)


add_executable(ARCVM-V2 main.cpp)

include(CTest)
enable_testing()

add_executable(test0 tests/test0.cpp)
add_test(Test0 test0)