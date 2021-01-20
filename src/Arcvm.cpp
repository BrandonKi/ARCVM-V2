#include "Arcvm.h"

Arcvm::Arcvm():
    program_counter(0), base_pointer(0)
{
    heap = (u8*)malloc(10); //TODO actually implement support for the heap
}

Arcvm::~Arcvm() {
    free(heap);
}

bool Arcvm::loadProgram(char* program, size_t size) {
    this->program = reinterpret<u8*>(program);
    this->size = size;
    if(verifySignature() == false)
        return false;
    return true;
}

inline bool Arcvm::verifySignature() {
    const char* signature = "\xfa\xca\xde";
    program += 3;
    return memcmp(signature, program - 3, 3) == 0;
}

i32 Arcvm::run() {

    while(program_counter != size) {
        execute();
        ++program_counter;
    }

    return exit_code;
}

void Arcvm::execute() {
    switch (program[program_counter]) {
        case instruction::exit:
        {
            exit_code = reinterpret<i32>(static_cast<u32>(stack.back()));   //FIXME this is just temporary so tests will pass and because the stack frame and functions aren't implemented yet
            break;
        }
        case instruction::ret:
        {
            exit_code = reinterpret<i32>(static_cast<u32>(stack.back()));   //FIXME this is just temporary so tests will pass and because the stack frame and functions aren't implemented yet
            break;
        }
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
        case instruction::call_short:
        {
            // old return address
            stack.push_back(program_counter);
            // old base pointer
            stack.push_back(base_pointer);
            // the new base pointer is equal to the current stack pointer
            base_pointer = stack_pointer();
            u8 jump_address = reinterpret<u8>(*nextByte());
            u32 local_variable_space = *reinterpret<u32*>(nextByte());
            program_counter += 3;
            stack.resize(stack.size() + local_variable_space, 0);
            jump(jump_address);
            break;
        }
        case instruction::call_long:
        {
            break;
        }

        default:
            break;
    }
}
