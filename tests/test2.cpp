#include "test.h"

/**
 * create a bitmask for the bottom three bits in three steps(one per bit)
 */
int main(int, char**) {

    std::vector<u8> program = {
        SIGNATURE,
        
        // init section
        vm::push_value_unsigned_8,
        0x00,
        vm::call_short,
        0x00,
        0x06,
        vm::exit,

        // code section
        vm::push_value_unsigned_8,
        0x00,
        vm::push_value_unsigned_8,
        0x01,
        vm::oru,
        vm::push_value_unsigned_8,
        0x02,
        vm::oru,
        vm::push_value_unsigned_8,
        0x04,
        vm::oru,

        vm::ret
    };
    expect(7u);
    return runTest(program);
}