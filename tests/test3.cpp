#include "test.h"

/**
 * put two 64 bit integers in registers then and them together
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
        vm::push_value_signed_64,
        0x0a,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        vm::pop_register,
        0x00,
        vm::push_value_signed_64,
        0x08,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        vm::pop_register,
        0x01,
        vm::ands_register_register,
        0x00,
        0x01,
        vm::push_register,
        0x00,


        vm::ret
    };
    expect(8);
    return runTest(program);
}