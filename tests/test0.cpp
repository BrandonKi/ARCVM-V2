#include "test.h"

/**
 * push two unsigned 64 bit values to the stack and add them
 */
int main(int, char**) {
    
    std::vector<u8> program = {
        SIGNATURE,
        
        // init section
        vm::call_short,
        0x07,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        vm::exit,

        // code section
        vm::push_value,
        0x64,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        vm::push_value,
        0x64,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        vm::addu,
        vm::ret
    };
    expect<u32>(200);
    return runTest(program);
}