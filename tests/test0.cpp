#include "ARCVM.h"

/**
 * push two unsigned 64 bit values to the stack and add them
 */
#define vm Arcvm::instruction
int main(int, char**) {
    
    char program[] = {
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

    Arcvm VM;
    VM.loadProgram(program, sizeof(program));
    return VM.run() == 200 ? 0 : 1;
}