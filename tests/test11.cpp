#include "test.h"

/**
 * test local variables
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
        vm::allocate_locals,
        0x01,
        vm::push_value_unsigned_8,
        0x05,
        vm::set_local,
        0x01,
        vm::load_local,
        0x01,
        vm::load_local,
        0x01,
        vm::addu,
        vm::deallocate_locals,
        0x01,
        vm::ret,
    };

    expect<u32>(10);
    return runTest(program);
}