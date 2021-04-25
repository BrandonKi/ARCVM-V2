#include "test.h"

/**
 * recursive fib sequence
 */
int main(int, char**) {
    
    std::vector<u8> program = {
        SIGNATURE,
        
        // init section
        vm::push_value_unsigned_8,
        0x00,
        vm::push_value_unsigned_8,
        0x0a,
        vm::call_short,
        0x01,
        0x08,
        vm::exit,

        // code section
        vm::load_arg,
        0x00,
        vm::push_value_unsigned_8,
        0x01,
        vm::gtu,
        vm::jump_ifnzero,
        0x12,
        vm::load_arg,
        0x00,
        vm::ret,
        vm::load_arg,
        0x00,
        vm::push_value_unsigned_8,
        0x01,
        vm::subu,
        vm::call_short,
        0x01,
        0x08,
        vm::load_arg,
        0x00,
        vm::push_value_unsigned_8,
        0x02,
        vm::subu,
        vm::call_short,
        0x01,
        0x08,
        vm::addu,
        vm::ret,
    };
    expect<u32>(55);
    return runTest(program);
}

// equivalent implementation in python
// def recur_fibo(n):
//    if n <= 1:
//        return n
//    else:
//        return(recur_fibo(n-1) + recur_fibo(n-2))