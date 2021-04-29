#include "test.h"

/**
 * test adding strings
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
        vm::push_string,
        0x05,
        0x00,
        0x00,
        0x00,
        'H',
        'e',
        'l',
        'l',
        'o',
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
        vm::string_add,
        vm::string_len,
        vm::ret,
    };

    expect<u32>(10);
    return runTest(program);
}