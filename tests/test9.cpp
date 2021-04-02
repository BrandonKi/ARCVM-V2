// It's fine if this test breaks
#include "test.h"

/**
 * test floating point values and arithmetic
 */
int main(int, char**) {
    
    std::vector<u8> program = {
        SIGNATURE,
        
        // init section
        vm::push_value_unsigned_8,
        0x00,
        vm::call_short,
        0x00,
        0x0a,
        0x00,
        0x00,
        0x00,
        0x00,
        vm::exit,

        // code section
        vm::push_value_float_64,
        0xcd,
        0xcc,
        0xcc,
        0xcc,
        0xcc,
        0xcc,
        0xfc,
        0x3f,
        vm::push_value_float_32,
        0xcd,
        0xcc,
        0xcc,
        0x3d,
        vm::addf,
        vm::ret,
    };
    // yea.....
    // this test is bound to break
    // this literal is equivalent to the bottom 32 bits of 
    // 0.1 + 1.8
    expect<i32>(1724697805);
    return runTest(program);
}