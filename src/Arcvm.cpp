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

    while(program_counter <= size) {
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
            
            // set the program counter equal to the max value so the program ends
            // I should probably change this in the future
            program_counter = size;                     
            break;
        }
        case instruction::ret:
        {
            u64 num_of_params = stack[static_cast<size_t>(base_pointer - 2)];
            u64 return_address = stack[static_cast<size_t>(base_pointer - 1)];
            u64 new_base_pointer = stack[static_cast<size_t>(base_pointer)];
            stack.erase(stack.begin() + static_cast<i32>(base_pointer - num_of_params - 2), stack.end() - 1);
            base_pointer = new_base_pointer;
            jump(return_address);
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
        case push_value_float_32:   //TODO THIS DOES NOT EVEN COME CLOSE TO WORKING
        {
            f32 value = *reinterpret<f32*>(nextByte());
            program_counter += 3; // move forwards 3 bytes to get past the immediate value
            stack.push_back(reinterpret<u64>(value));
            break;
        }
        case push_value_float_64:
        {
            f64 value = *reinterpret<f64*>(nextByte());
            program_counter += 7; // move forwards 7 bytes to get past the immediate value
            stack.push_back(reinterpret<u64>(value));
            break;
        }
        case push_value_1:
        {
            u8 value = (*nextByte()) & 0x01;
            stack.push_back(static_cast<u64>(value));
            break;
        }
        case push_value_signed_8:   //TODO THIS DOES NOT EVEN COME CLOSE TO WORKING
        {
            i8 value = reinterpret<i8>(*nextByte());
            stack.push_back(reinterpret<u64>(value));
            break;
        }
        case push_value_signed_16:   //TODO THIS DOES NOT EVEN COME CLOSE TO WORKING
        {
            i16 value = *reinterpret<i16*>(nextByte());
            ++program_counter; // move forwards 1 byte to get past the immediate value
            stack.push_back(reinterpret<u64>(value));
            break;
        }
        case push_value_signed_32:   //TODO THIS DOES NOT EVEN COME CLOSE TO WORKING
        {
            i32 value = *reinterpret<i32*>(nextByte());
            program_counter += 3; // move forwards 3 bytes to get past the immediate value
            stack.push_back(reinterpret<u64>(value));
            break;
        }
        case push_value_signed_64:
        {
            i64 value = *reinterpret<i64*>(nextByte());
            program_counter += 7; // move forwards 7 bytes to get past the immediate value
            stack.push_back(reinterpret<u64>(value));
            break;
        }
        case push_value_unsigned_8:
        {
            u8 value = *nextByte();
            stack.push_back(static_cast<u64>(value));
            break;
        }
        case push_value_unsigned_16:
        {
            u16 value = *reinterpret<u16*>(nextByte());
            ++program_counter; // move forwards 1 byte to get past the immediate value
            stack.push_back(static_cast<u64>(value));
            break;
        }
        case push_value_unsigned_32:
        {
            u32 value = *reinterpret<u32*>(nextByte());
            program_counter += 3; // move forwards 3 bytes to get past the immediate value
            stack.push_back(static_cast<u64>(value));
            break;
        }
        case push_value_unsigned_64:
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
            Register *reg = toRegister(*nextByte());
            stack.push_back(reg->x64);
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
            Register *dest = toRegister(*nextByte());
            Register *src = toRegister(*nextByte());
            dest->x64 = dest->x64 + src->x64;
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
            Register *dest = toRegister(*nextByte());
            Register *src = toRegister(*nextByte());
            // add the two value as a signed integer then store as unsigned
            dest->x64 = reinterpret<u64>(reinterpret<i64>(dest->x64) + reinterpret<i64>(src->x64));
            
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
            Register *dest = toRegister(*nextByte());
            Register *src = toRegister(*nextByte());

            dest->x64 = dest->x64 - src->x64;
            
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
            Register *dest = toRegister(*nextByte());
            Register *src = toRegister(*nextByte());
            // add the two value as a signed integer then store as unsigned
            dest->x64 = reinterpret<u64>(reinterpret<i64>(dest->x64) - reinterpret<i64>(src->x64));
            
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
            Register *dest = toRegister(*nextByte());
            Register *src = toRegister(*nextByte());
            dest->x64 = dest->x64 * src->x64;
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
            Register *dest = toRegister(*nextByte());
            Register *src = toRegister(*nextByte());
            // add the two value as a signed integer then store as unsigned
            dest->x64 = reinterpret<u64>(reinterpret<i64>(dest->x64) * reinterpret<i64>(src->x64));
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
            Register *dest = toRegister(*nextByte());
            Register *src = toRegister(*nextByte());
            dest->x64 = dest->x64 / src->x64;
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
            Register *dest = toRegister(*nextByte());
            Register *src = toRegister(*nextByte());
            // add the two value as a signed integer then store as unsigned
            dest->x64 = reinterpret<u64>(reinterpret<i64>(dest->x64) / reinterpret<i64>(src->x64));
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
            Register *dest = toRegister(*nextByte());
            Register *src = toRegister(*nextByte());
            dest->x64 = dest->x64 % src->x64;
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
            Register *dest = toRegister(*nextByte());
            Register *src = toRegister(*nextByte());
            // add the two value as a signed integer then store as unsigned
            dest->x64 = reinterpret<u64>(reinterpret<i64>(dest->x64) % reinterpret<i64>(src->x64));
            break;        
        }
        case instruction::andu:
        {
            u64 op1 = stack.back();
            stack.pop_back();
            u64 op2 = stack.back();
            stack.pop_back();
            stack.push_back(op2 & op1);
            break;
        }
        case instruction::andu_register_register:
        {
            Register *dest = toRegister(*nextByte());
            Register *src = toRegister(*nextByte());
            dest->x64 = dest->x64 & src->x64;
            break;
        }
        case instruction::ands:
        {
            i64 op1 = reinterpret<i64>(stack.back());
            stack.pop_back();
            i64 op2 = reinterpret<i64>(stack.back());
            stack.pop_back();
            stack.push_back(reinterpret<u64>(op2 & op1));
            break;
        }
        case instruction::ands_register_register:
        {
            Register *dest = toRegister(*nextByte());
            Register *src = toRegister(*nextByte());
            // add the two value as a signed integer then store as unsigned
            dest->x64 = reinterpret<u64>(reinterpret<i64>(dest->x64) & reinterpret<i64>(src->x64));
            break;        
        }
        case instruction::oru:
        {
            u64 op1 = stack.back();
            stack.pop_back();
            u64 op2 = stack.back();
            stack.pop_back();
            stack.push_back(op2 | op1);
            break;
        }
        case instruction::oru_register_register:
        {
            Register *dest = toRegister(*nextByte());
            Register *src = toRegister(*nextByte());
            dest->x64 = dest->x64 | src->x64;
            break;
        }
        case instruction::ors:
        {
            i64 op1 = reinterpret<i64>(stack.back());
            stack.pop_back();
            i64 op2 = reinterpret<i64>(stack.back());
            stack.pop_back();
            stack.push_back(reinterpret<u64>(op2 | op1));
            break;
        }
        case instruction::ors_register_register:
        {
            Register *dest = toRegister(*nextByte());
            Register *src = toRegister(*nextByte());
            // add the two value as a signed integer then store as unsigned
            dest->x64 = reinterpret<u64>(reinterpret<i64>(dest->x64) | reinterpret<i64>(src->x64));
            break;        
        }
        case instruction::equalu:
        {
            u64 op1 = stack.back();
            stack.pop_back();
            u64 op2 = stack.back();
            stack.pop_back();
            stack.push_back(op2 == op1);
            break;
        }
        case instruction::equalu_register_register:
        {
            Register *dest = toRegister(*nextByte());
            Register *src = toRegister(*nextByte());
            stack.push_back(dest->x64 == src->x64);
            break;
        }
        case instruction::equals:
        {
            i64 op1 = reinterpret<i64>(stack.back());
            stack.pop_back();
            i64 op2 = reinterpret<i64>(stack.back());
            stack.pop_back();
            stack.push_back(reinterpret<u64>(op2 == op1));
            break;
        }
        case instruction::equals_register_register:
        {
            Register *dest = toRegister(*nextByte());
            Register *src = toRegister(*nextByte());
            // add the two value as a signed integer then store as unsigned
            stack.push_back(reinterpret<u64>(reinterpret<i64>(dest->x64) == reinterpret<i64>(src->x64)));
            break;        
        }
        case instruction::not_equalu:
        {
            u64 op1 = stack.back();
            stack.pop_back();
            u64 op2 = stack.back();
            stack.pop_back();
            stack.push_back(op2 != op1);
            break;
        }
        case instruction::not_equalu_register_register:
        {
            Register *dest = toRegister(*nextByte());
            Register *src = toRegister(*nextByte());
            stack.push_back(dest->x64 != src->x64);
            break;
        }
        case instruction::not_equals:
        {
            i64 op1 = reinterpret<i64>(stack.back());
            stack.pop_back();
            i64 op2 = reinterpret<i64>(stack.back());
            stack.pop_back();
            stack.push_back(reinterpret<u64>(op2 != op1));
            break;
        }
        case instruction::not_equals_register_register:
        {
            Register *dest = toRegister(*nextByte());
            Register *src = toRegister(*nextByte());
            // add the two value as a signed integer then store as unsigned
            stack.push_back(reinterpret<u64>(reinterpret<i64>(dest->x64) != reinterpret<i64>(src->x64)));
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
        case instruction::jump_ifzero:
        {
            if(stack.back() == 0)
                jump(reinterpret<u8>(*nextByte()));
            break;
        }
        case instruction::jump_ifnzero:
        {
            if(stack.back() != 0)
                jump(reinterpret<u8>(*nextByte()));
            break;
        }
        case instruction::call_short:
        {
            u8 jump_address = reinterpret<u8>(*nextByte());
            u32 local_variable_space = *reinterpret<u32*>(nextByte());
            program_counter += 3;
            // old return address
            stack.push_back(program_counter + 1);   // add one more to the program counter because we want jump to the next instruction when returning
            // old base pointer
            stack.push_back(base_pointer);
            // the new base pointer is equal to the current stack pointer
            base_pointer = stack_pointer();
            stack.resize(stack.size() + local_variable_space, 0);
            jump(jump_address);
            break;
        }
        case instruction::call_long:
        {
            u32 jump_address = reinterpret<u32>(*nextByte());
            u32 local_variable_space = *reinterpret<u32*>(nextByte());
            program_counter += 3;
            // old return address
            stack.push_back(program_counter + 1);   // add one more to the program counter because we want jump to the next instruction when returning
            // old base pointer
            stack.push_back(base_pointer);
            // the new base pointer is equal to the current stack pointer
            base_pointer = stack_pointer();
            stack.resize(stack.size() + local_variable_space, 0);
            jump(jump_address);
            break;
        }

        default:
            break;
    }
}
