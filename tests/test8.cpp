#include "test.h"

/**
 * test signed integer arithmetic
 * on values that are not 64 bit
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
        vm::push_value_signed_32,
        0xff,
        0xff,
        0xff,
        0xff,
        vm::push_value_signed_16,
        0xff,
        0xff,
        vm::adds,
        vm::push_value_signed_8,
        0xff,
        vm::adds,
        vm::push_value_signed_8,
        0xff,
        vm::adds,
        vm::push_value_signed_8,
        0xfe,
        vm::subs,
        vm::push_value_signed_8,
        0x02,
        vm::muls,
        vm::ret,

        
    };
    expect<i32>(-4);
    return runTest(program);
}