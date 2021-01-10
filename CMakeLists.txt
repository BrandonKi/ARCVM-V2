cmake_minimum_required(VERSION 3.0.0)
project(ARCVM-V2 VERSION 0.1.0)

include_directories(${CMAKE_SOURCE_DIR}/include)

set(sources 
    src/main.cpp
    src/ARCVM.cpp
)

add_executable(ARCVM-V2 ${sources})

set_target_properties( ARCVM-V2
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)

include(CTest)
enable_testing()

add_executable(test0 tests/test0.cpp)
add_test(Test0 test0)

set_target_properties( test0
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/tests/bin"
)