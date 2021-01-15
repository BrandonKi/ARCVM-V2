#include "test.h"

/**
 * push two unsigned 64 bit values to the stack and add them
 */
int main(int, char**) {
    
    std::vector<char> program = {
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