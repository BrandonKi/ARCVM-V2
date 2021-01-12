#include "Arcvm.h"

Arcvm::Arcvm():
    stack_pointer(0), program_counter(0), frame_pointer(0)
{
    heap = (u8*)malloc(100000);
}

Arcvm::~Arcvm() {
    free(heap);
}

void Arcvm::loadProgram(char* program, size_t size) {
    this->program = reinterpret<u8*>(program);
    this->size = size;
}

i32 Arcvm::run() {

    while(program_counter != size) {
        execute();
        ++program_counter;
    }

    return registers[0].x32;
}

void Arcvm::execute() {
    switch (program[program_counter]) {
        case instruction::ret:
            break;
        case instruction::mov_register_value:
        {
            u8 reg = program[++program_counter];
            u64 value = *reinterpret<u64*>(&program[++program_counter]);
            program_counter += 7; // move forwards 7 bytes to get past the immediate value
            registers[reg].x64 = value;
            break;
        }
        case instruction::mov_register_address: 
            break;
        case instruction::mov_register_register:
        {
            u8 dest = program[++program_counter];
            u8 src = program[++program_counter];
            registers[dest] = registers[src];
            break;
        }
        case instruction::mov_address_value:
            break;
        case instruction::mov_address_address:
            break;
        case instruction::mov_address_register:
            break;
        case instruction::push_value:
            u64 value = *reinterpret<u64*>(&program[++program_counter]);
            program_counter += 7; // move forwards 7 bytes to get past the immediate value
            stack.push_back(value);
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
