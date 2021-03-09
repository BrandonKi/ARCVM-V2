#include "test.h"

/**
 * test using function return values
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
        vm::push_value_unsigned_8,
        0x00,
        vm::call_short,
        0x01,
        0x17,
        0x00,
        0x00,
        0x00,
        0x00,
        vm::push_value_unsigned_8,
        0x01,
        vm::addu,
        vm::ret,

        // add one to the parameter
        // function(unsigned int) -> unsigned int
        vm::push_param,
        0x00,
        vm::push_value_unsigned_8,
        0x01,
        vm::addu,
        vm::ret,
        
    };
    expect<u32>(2);
    return runTest(program);
}