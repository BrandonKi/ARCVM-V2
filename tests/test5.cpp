#include "test.h"

/**
 * push two unsigned 64 bit values to the stack and add them
 */
int main(int, char**) {
    
    std::vector<u8> program = {
        SIGNATURE,
        
        // init section
        vm::push_value_unsigned_8,
        0x00,
        vm::call_short,
        0x09,
        0x00,
        0x00,
        0x00,
        0x00,
        vm::exit,

        // code section
        vm::push_value_unsigned_8,
        0x64,
        vm::push_value_unsigned_8,
        0x64,
        vm::equalu,
        vm::push_value_unsigned_8,
        0x64,
        vm::push_value_unsigned_8,
        0x64,
        vm::equalu,
        vm::equalu,
        vm::ret
    };
    expect<u32>(1);
    return runTest(program);
}