#include "test.h"

/**
 * test strings
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
        vm::push_string,
        'H',
        'i',
        vm::push_string,
        'W',
        'o',
        'r',
        'l',
        'd',
        vm::ret,
    };
    // yea.....
    // this test is bound to break
    // this literal is equivalent to the bottom 32 bits of 
    // 0.1 + 1.8
    expect<i32>(1724697805);
    return runTest(program);
}