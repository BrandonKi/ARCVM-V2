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
        0x02,
        0x00,
        0x00,
        0x00,
        'H',
        'i',
        vm::push_string,
        0x05,
        0x00,
        0x00,
        0x00,
        'W',
        'o',
        'r',
        'l',
        'd',
        vm::string_len,
        vm::ret,
    };

    expect<u32>(5);
    return runTest(program);
}