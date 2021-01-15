cmake_minimum_required(VERSION 3.0.0)
project(ARCVM-V2 VERSION 0.1.0)

include_directories(${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/tests)

cmake_policy(SET CMP0054 NEW)

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

set(test_bin_dir ${CMAKE_SOURCE_DIR}/tests/bin)


add_executable(test0 tests/test0.cpp src/Arcvm.cpp)
set_target_properties( test0
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${test_bin_dir}
)

add_executable(test1 tests/test1.cpp src/Arcvm.cpp)
set_target_properties( test1
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${test_bin_dir}
)

# This mess of if statements is to get around a bug in the extension I'm using for the test UI
# when cmake is run with -DFIX_TEST_UI_BUG=True then this manually finds the test binaries otherwise I let cmake handle that
if(${FIX_TEST_UI_BUG})
    if(CMAKE_CXX_COMPILER_ID EQUAL "MSVC")
        if(CMAKE_BUILD_TYPE EQUAL "DEBUG")
            set(test_exe_path ${test_bin_dir}/Debug)
        else()
            set(test_exe_path ${test_bin_dir}/Release)
        endif()
    else()
        set(test_exe_path ${test_bin_dir})
    endif()
    add_test(Test0 ${test_exe_path}/test0)
    add_test(Test1 ${test_exe_path}/test1)
else()
    add_test(NAME Test0 COMMAND test0)
    add_test(NAME Test1 COMMAND test1)
endif()
