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

add_executable(test2 tests/test2.cpp src/Arcvm.cpp)
set_target_properties( test2
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${test_bin_dir}
)

add_executable(test3 tests/test3.cpp src/Arcvm.cpp)
set_target_properties( test3
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${test_bin_dir}
)

add_executable(test4 tests/test4.cpp src/Arcvm.cpp)
set_target_properties( test4
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${test_bin_dir}
)

add_executable(test5 tests/test5.cpp src/Arcvm.cpp)
set_target_properties( test5
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${test_bin_dir}
)

add_executable(test6 tests/test6.cpp src/Arcvm.cpp)
set_target_properties( test6
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${test_bin_dir}
)

add_executable(test7 tests/test7.cpp src/Arcvm.cpp)
set_target_properties( test7
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${test_bin_dir}
)

add_executable(testfib tests/testfib.cpp src/Arcvm.cpp)
set_target_properties( testfib
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
    add_test(Test2 ${test_exe_path}/test2)
    add_test(Test3 ${test_exe_path}/test3)
    add_test(Test4 ${test_exe_path}/test4)
    add_test(Test5 ${test_exe_path}/test5)
    add_test(Test6 ${test_exe_path}/test6)
    add_test(Test7 ${test_exe_path}/test7)
    add_test(Testfib ${test_exe_path}/testfib)
else()
    add_test(NAME Test0 COMMAND test0)
    add_test(NAME Test1 COMMAND test1)
    add_test(NAME Test2 COMMAND test2)
    add_test(NAME Test3 COMMAND test3)
    add_test(NAME Test4 COMMAND test4)
    add_test(NAME Test5 COMMAND test5)
    add_test(NAME Test6 COMMAND test6)
    add_test(NAME Testfib COMMAND testfib)
endif()
