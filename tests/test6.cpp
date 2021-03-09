#include "test.h"

/**
 * test passing parameters
 */
int main(int, char**) {
    
    std::vector<u8> program = {
        SIGNATURE,
        
        // init section
        vm::push_value_unsigned_8,
        0x00,
        vm::push_value_unsigned_8,
        0x05,
        vm::push_value_unsigned_8,
        0x0a,
        vm::call_short,
        0x02,
        0x0e,
        0x00,
        0x00,
        0x00,
        0x00,
        vm::exit,

        // code section
        vm::push_param,
        0x00,
        vm::push_param,
        0x01,
        vm::addu,
        vm::ret
    };
    expect<u32>(15);
    return runTest(program);
}