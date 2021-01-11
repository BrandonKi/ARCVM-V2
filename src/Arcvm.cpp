#include "Arcvm.h"

Arcvm::Arcvm() {
    heap = (u8*)malloc(100000);
}

Arcvm::~Arcvm() {
    free(heap);
}

void Arcvm::loadProgram(const u8 *program, size_t size) {
    this->program = program;
    this->size = size;
}

i32 Arcvm::run() {

    while(true) {
        ++program_counter;
        execute();
    }

    return 0;
}

void Arcvm::execute() {
    switch (program[program_counter]) {
        case instruction::ret:
            break;
        case instruction::mov_register_value:

            break;
        case instruction::mov_register_address:
            break;
        case instruction::mov_register_register:
            
            break;
        case instruction::mov_address_value:
            break;
        case instruction::mov_address_address:
            break;
        case instruction::mov_address_register:
            break;
        case instruction::push_value:
            break;
        case instruction::push_address:
            break;
        case instruction::push_register:
            break;
        case instruction::pop_register:
            break;
        case instruction::pop_address: 
            break;
        case instruction::add:
            break;
        case instruction::add_register_register:
            break;
        case instruction::sub:
            break;
        case instruction::sub_register_register:
            break;
        case instruction::mul:
            break;
        case instruction::mul_register_register:
            break;
        case instruction::div:
            break;
        case instruction::div_register_register:
            break;
        case instruction::mod:
            break;
        case instruction::mod_register_register:
            break;
        case instruction::jump:
            break;
        case instruction::call:
            break;
        default:
            break;
    }
}
