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
            u8 reg_num = *nextByte();
            u64 value = *reinterpret<u64*>(nextByte());
            program_counter += 7; // move forwards 7 bytes to get past the immediate value
            registers[reg_num].x64 = value;
            break;
        }
        case instruction::mov_register_address:
            break;
        case instruction::mov_register_register:
        {
            u8 dest = *nextByte();
            u8 src = *nextByte();
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
        {
            u64 value = *reinterpret<u64*>(nextByte());
            program_counter += 7; // move forwards 7 bytes to get past the immediate value
            stack.push_back(value);
            break;
        }
        case instruction::push_address:
            break;
        case instruction::push_register:
        {
            u8 reg_num = *nextByte();
            stack.push_back(registers[reg_num].x64);
            break;
        }
        case instruction::pop_register:
        {
            u8 reg_num = *nextByte();
            registers[reg_num].x64 = stack.back();
            stack.pop_back(); 
            break;
        }
        case instruction::pop_address:
        {
            break;
        }
        case instruction::addu:
        {
            u64 op1 = stack.back();
            stack.pop_back();
            u64 op2 = stack.back();
            stack.pop_back();
            stack.push_back(op2 + op1);
            break;
        }
        case instruction::addu_register_register:
        {
            Register dest = toRegister(*nextByte());
            Register src = toRegister(*nextByte());
            dest.x64 = dest.x64 + src.x64;
            break;
        }

        case instruction::adds:
        {
            i64 op1 = reinterpret<i64>(stack.back());
            stack.pop_back();
            i64 op2 = reinterpret<i64>(stack.back());
            stack.pop_back();
            stack.push_back(reinterpret<u64>(op2 + op1));
            break;
        }
        case instruction::adds_register_register:
        {
            Register dest = toRegister(*nextByte());
            Register src = toRegister(*nextByte());
            // add the two value as a signed integer then store as unsigned
            dest.x64 = reinterpret<u64>(reinterpret<i64>(dest.x64) + reinterpret<i64>(src.x64));
            
            break;
        }
        case instruction::subu:
        {
            u64 op1 = stack.back();
            stack.pop_back();
            u64 op2 = stack.back();
            stack.pop_back();
            stack.push_back(op2 - op1);
            break;
        }
        case instruction::subu_register_register:
        {
            Register dest = toRegister(*nextByte());
            Register src = toRegister(*nextByte());

            dest.x64 = dest.x64 - src.x64;
            
            break;
        }
        case instruction::subs:
        {
            i64 op1 = reinterpret<i64>(stack.back());
            stack.pop_back();
            i64 op2 = reinterpret<i64>(stack.back());
            stack.pop_back();
            stack.push_back(reinterpret<u64>(op2 - op1));
            break;
        }
        case instruction::subs_register_register:
        {
            Register dest = toRegister(*nextByte());
            Register src = toRegister(*nextByte());
            // add the two value as a signed integer then store as unsigned
            dest.x64 = reinterpret<u64>(reinterpret<i64>(dest.x64) - reinterpret<i64>(src.x64));
            
            break;
        }
        case instruction::mulu:
        {
            u64 op1 = stack.back();
            stack.pop_back();
            u64 op2 = stack.back();
            stack.pop_back();
            stack.push_back(op2 * op1);
            break;
        }
        case instruction::mulu_register_register:
        {
            Register dest = toRegister(*nextByte());
            Register src = toRegister(*nextByte());
            dest.x64 = dest.x64 * src.x64;
            break;
        }
        case instruction::muls:
        {
            i64 op1 = reinterpret<i64>(stack.back());
            stack.pop_back();
            i64 op2 = reinterpret<i64>(stack.back());
            stack.pop_back();
            stack.push_back(reinterpret<u64>(op2 * op1));
            break;
        }
        case instruction::muls_register_register:
        {
            Register dest = toRegister(*nextByte());
            Register src = toRegister(*nextByte());
            // add the two value as a signed integer then store as unsigned
            dest.x64 = reinterpret<u64>(reinterpret<i64>(dest.x64) * reinterpret<i64>(src.x64));
            break;
        }
        case instruction::divu:
        {
            u64 op1 = stack.back();
            stack.pop_back();
            u64 op2 = stack.back();
            stack.pop_back();
            stack.push_back(op2 / op1);
            break;
        }
        case instruction::divu_register_register:
        {
            Register dest = toRegister(*nextByte());
            Register src = toRegister(*nextByte());
            dest.x64 = dest.x64 / src.x64;
            break;
        }
        case instruction::divs:
        {
            i64 op1 = reinterpret<i64>(stack.back());
            stack.pop_back();
            i64 op2 = reinterpret<i64>(stack.back());
            stack.pop_back();
            stack.push_back(reinterpret<u64>(op2 / op1));
            break;
        }
        case instruction::divs_register_register:
        {
            Register dest = toRegister(*nextByte());
            Register src = toRegister(*nextByte());
            // add the two value as a signed integer then store as unsigned
            dest.x64 = reinterpret<u64>(reinterpret<i64>(dest.x64) / reinterpret<i64>(src.x64));
            break;
        }
        case instruction::modu:
        {
            u64 op1 = stack.back();
            stack.pop_back();
            u64 op2 = stack.back();
            stack.pop_back();
            stack.push_back(op2 % op1);
            break;
        }
        case instruction::modu_register_register:
        {
            Register dest = toRegister(*nextByte());
            Register src = toRegister(*nextByte());
            dest.x64 = dest.x64 % src.x64;
            break;
        }
        case instruction::mods:
        {
            i64 op1 = reinterpret<i64>(stack.back());
            stack.pop_back();
            i64 op2 = reinterpret<i64>(stack.back());
            stack.pop_back();
            stack.push_back(reinterpret<u64>(op2 % op1));
            break;
        }
        case instruction::mods_register_register:
        {
            Register dest = toRegister(*nextByte());
            Register src = toRegister(*nextByte());
            // add the two value as a signed integer then store as unsigned
            dest.x64 = reinterpret<u64>(reinterpret<i64>(dest.x64) % reinterpret<i64>(src.x64));
            break;        
        }
        case instruction::jump_short:
        {
            break;
        }
        case instruction::jump_long:
        {
            break;
        }
        case instruction::call:
        {
            break;
        }
        default:
            break;
    }
}
