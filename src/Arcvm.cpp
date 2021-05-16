#include "Arcvm.h"

Arcvm::Arcvm():
    program_counter_(0), base_pointer_(0), program_(nullptr), size_(0), exit_code_(0)
{
    PROFILE();
    heap_ = static_cast<u8*>(malloc(10)); //TODO actually implement support for the heap
}

Arcvm::~Arcvm() {
    PROFILE();
    free(heap_);
}

bool Arcvm::load_program(char* program, const size_t size) {
    PROFILE();
    this->program_ = reinterpret<u8*>(program);
    this->size_ = size;
    if(!verifySignature())
        return false;
    return true;
}

inline bool Arcvm::verifySignature() {
    PROFILE();
    const char* signature = "\xfa\xca\xde";
    const int sig_len = 3;
    program_ += sig_len;
    return memcmp(signature, program_ - sig_len, sig_len) == 0;
}

i32 Arcvm::run() {
    PROFILE();

    while(program_counter_ <= size_) {
        execute();
        ++program_counter_;
    }

    return exit_code_;
}

void Arcvm::execute() {
    PROFILE();
    switch (program_[program_counter_]) {
        case instruction::exit:
        {
            PROFILE_SCOPE("exit");
            
            exit_code_ = reinterpret<i32>(static_cast<u32>(stack_.back() & 0x00000000ffffffff));   //FIXME this is just temporary so tests will pass and because the stack frame and functions aren't implemented yet
            
            // set the program counter equal to the max value so the program ends
            // I should probably change this in the future
            program_counter_ = size_;
            break;
        }
        case instruction::ret:
        {
            PROFILE_SCOPE("ret");
            u64 num_of_params = stack_[static_cast<size_t>(base_pointer_ - 2)];
            u64 return_address = stack_[static_cast<size_t>(base_pointer_ - 1)];
            u64 new_base_pointer = stack_[static_cast<size_t>(base_pointer_)];
            stack_.erase(stack_.begin() + static_cast<i32>(base_pointer_ - num_of_params - 2), stack_.end() - 1);
            base_pointer_ = new_base_pointer;
            jump(return_address);
            break;
        }
        case instruction::mov_register_value:
        {
            PROFILE_SCOPE("mov_register_value");
            auto reg_num = *next_byte();
            auto value = *reinterpret<u64*>(next_byte());
            program_counter_ += 7; // move forwards 7 bytes to get past the immediate value
            registers_[reg_num].x64 = value;
            break;
        }
        case instruction::mov_register_address:
            break;
        case instruction::mov_register_register:
        {
            PROFILE_SCOPE("mov_register_register");
            auto dest = *next_byte();
            auto src = *next_byte();
            registers_[dest] = registers_[src];
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
            PROFILE_SCOPE("push_value");
            auto value = *reinterpret<u64*>(next_byte());
            program_counter_ += 7; // move forwards 7 bytes to get past the immediate value
            stack_.push_back(value);
            break;
        }
        case push_value_float_32:
        {
            PROFILE_SCOPE("push_value_float_32");
            auto value = static_cast<f64>(*reinterpret<f32*>(next_byte()));
            program_counter_ += 3; // move forwards 3 bytes to get past the immediate value
            stack_.push_back(reinterpret<u64>(value));
            break;
        }
        case push_value_float_64:
        {
            PROFILE_SCOPE("push_value_float_64");
            auto value = *reinterpret<f64*>(next_byte());
            program_counter_ += 7; // move forwards 7 bytes to get past the immediate value
            stack_.push_back(reinterpret<u64>(value));
            break;
        }
        case push_value_1:
        {
            PROFILE_SCOPE("push_value_1");
            u8 value = (*next_byte()) & 0x01;
            stack_.push_back(static_cast<u64>(value));
            break;
        }
        case push_value_signed_8:
        {
            PROFILE_SCOPE("push_value_signed_8");
            auto value = static_cast<i64>(reinterpret<i8>(*next_byte()));
            stack_.push_back(reinterpret<u64>(value));
            break;
        }
        case push_value_signed_16:
        {
            PROFILE_SCOPE("push_value_signed_16");
            auto value = static_cast<i64>(*reinterpret<i16*>(next_byte()));
            ++program_counter_; // move forwards 1 byte to get past the immediate value
            stack_.push_back(reinterpret<u64>(value));
            break;
        }
        case push_value_signed_32:
        {
            PROFILE_SCOPE("push_value_signed_32");
            auto value = static_cast<i64>(*reinterpret<i32*>(next_byte()));
            program_counter_ += 3; // move forwards 3 bytes to get past the immediate value
            stack_.push_back(reinterpret<u64>(value));
            break;
        }
        case push_value_signed_64:
        {
            PROFILE_SCOPE("push_value_signed_64");
            auto value = *reinterpret<i64*>(next_byte());
            program_counter_ += 7; // move forwards 7 bytes to get past the immediate value
            stack_.push_back(reinterpret<u64>(value));
            break;
        }
        case push_value_unsigned_8:
        {
            PROFILE_SCOPE("push_value_unsigned_8");
            auto value = *next_byte();
            stack_.push_back(static_cast<u64>(value));
            break;
        }
        case push_value_unsigned_16:
        {
            PROFILE_SCOPE("push_value_unsigned_16");
            auto value = *reinterpret<u16*>(next_byte());
            ++program_counter_; // move forwards 1 byte to get past the immediate value
            stack_.push_back(static_cast<u64>(value));
            break;
        }
        case push_value_unsigned_32:
        {
            PROFILE_SCOPE("push_value_unsigned_32");
            auto value = *reinterpret<u32*>(next_byte());
            program_counter_ += 3; // move forwards 3 bytes to get past the immediate value
            stack_.push_back(static_cast<u64>(value));
            break;
        }
        case push_value_unsigned_64:
        {
            PROFILE_SCOPE("push_value_unsigned_64");
            auto value = *reinterpret<u64*>(next_byte());
            program_counter_ += 7; // move forwards 7 bytes to get past the immediate value
            stack_.push_back(value);
            break;
        }
        case instruction::push_address:
            break;
        case instruction::push_register:
        {
            PROFILE_SCOPE("push_register");
            Register* reg = to_register(*next_byte());
            stack_.push_back(reg->x64);
            break;
        }
        case instruction::pop_register:
        {
            PROFILE_SCOPE("pop_register");
            auto reg_num = *next_byte();
            registers_[reg_num].x64 = stack_.back();
            stack_.pop_back();
            break;
        }
        case instruction::pop_address:
        {
            PROFILE_SCOPE("pop_address");
            break;
        }
        case instruction::addu:
        {
            PROFILE_SCOPE("addu");
            auto op1 = stack_.back();
            stack_.pop_back();
            auto op2 = stack_.back();
            stack_.pop_back();
            stack_.push_back(op2 + op1);
            break;
        }
        case instruction::addu_register_register:
        {
            PROFILE_SCOPE("addu_register_register");
            Register* dest = to_register(*next_byte());
            Register* src = to_register(*next_byte());
            dest->x64 = dest->x64 + src->x64;
            break;
        }

        case instruction::adds:
        {
            PROFILE_SCOPE("adds");
            auto op1 = reinterpret<i64>(stack_.back());
            stack_.pop_back();
            auto op2 = reinterpret<i64>(stack_.back());
            stack_.pop_back();
            stack_.push_back(reinterpret<u64>(op2 + op1));
            break;
        }
        case instruction::adds_register_register:
        {
            PROFILE_SCOPE("adds_register_register");
            Register* dest = to_register(*next_byte());
            Register* src = to_register(*next_byte());
            // add the two value as a signed integer then store as unsigned
            dest->x64 = reinterpret<u64>(reinterpret<i64>(dest->x64) + reinterpret<i64>(src->x64));
            break;
        }
        case instruction::addf:
        {
            PROFILE_SCOPE("addf");
            auto op1 = reinterpret<f64>(stack_.back());
            stack_.pop_back();
            auto op2 = reinterpret<f64>(stack_.back());
            stack_.pop_back();
            stack_.push_back(reinterpret<u64>(op2 + op1));
            break;
        }
        case instruction::addf_register_register:
        {
            PROFILE_SCOPE("addf_register_register");
            Register* dest = to_register(*next_byte());
            Register* src = to_register(*next_byte());
            // add the two value as a signed integer then store as unsigned
            dest->x64 = reinterpret<u64>(reinterpret<f64>(dest->x64) + reinterpret<f64>(src->x64));
            break;
        }
        case instruction::subu:
        {
            PROFILE_SCOPE("subu");
            auto op1 = stack_.back();
            stack_.pop_back();
            auto op2 = stack_.back();
            stack_.pop_back();
            stack_.push_back(op2 - op1);
            break;
        }
        case instruction::subu_register_register:
        {
            PROFILE_SCOPE("subu_register_register");
            Register* dest = to_register(*next_byte());
            Register* src = to_register(*next_byte());

            dest->x64 = dest->x64 - src->x64;

            break;
        }
        case instruction::subs:
        {
            PROFILE_SCOPE("subs");
            auto op1 = reinterpret<i64>(stack_.back());
            stack_.pop_back();
            auto op2 = reinterpret<i64>(stack_.back());
            stack_.pop_back();
            stack_.push_back(reinterpret<u64>(op2 - op1));
            break;
        }
        case instruction::subs_register_register:
        {
            PROFILE_SCOPE("subs_register_register");
            Register* dest = to_register(*next_byte());
            Register* src = to_register(*next_byte());
            // add the two value as a signed integer then store as unsigned
            dest->x64 = reinterpret<u64>(reinterpret<i64>(dest->x64) - reinterpret<i64>(src->x64));
            break;
        }
        case instruction::subf:
        {
            PROFILE_SCOPE("subf");
            auto op1 = reinterpret<f64>(stack_.back());
            stack_.pop_back();
            auto op2 = reinterpret<f64>(stack_.back());
            stack_.pop_back();
            stack_.push_back(reinterpret<u64>(op2 - op1));
            break;
        }
        case instruction::subf_register_register:
        {
            PROFILE_SCOPE("subf_register_register");
            Register* dest = to_register(*next_byte());
            Register* src = to_register(*next_byte());
            // add the two value as a signed integer then store as unsigned
            dest->x64 = reinterpret<u64>(reinterpret<f64>(dest->x64) - reinterpret<f64>(src->x64));
            break;
        }
        case instruction::mulu:
        {
            PROFILE_SCOPE("mulu");
            auto op1 = stack_.back();
            stack_.pop_back();
            auto op2 = stack_.back();
            stack_.pop_back();
            stack_.push_back(op2 * op1);
            break;
        }
        case instruction::mulu_register_register:
        {
            PROFILE_SCOPE("mulu_register_register");
            Register* dest = to_register(*next_byte());
            Register* src = to_register(*next_byte());
            dest->x64 = dest->x64 * src->x64;
            break;
        }
        case instruction::muls:
        {
            PROFILE_SCOPE("muls");
            auto op1 = reinterpret<i64>(stack_.back());
            stack_.pop_back();
            auto op2 = reinterpret<i64>(stack_.back());
            stack_.pop_back();
            stack_.push_back(reinterpret<u64>(op2 * op1));
            break;
        }
        case instruction::muls_register_register:
        {
            PROFILE_SCOPE("muls_register_register");
            Register* dest = to_register(*next_byte());
            Register* src = to_register(*next_byte());
            // add the two value as a signed integer then store as unsigned
            dest->x64 = reinterpret<u64>(reinterpret<i64>(dest->x64) * reinterpret<i64>(src->x64));
            break;
        }
        case instruction::mulf:
        {
            PROFILE_SCOPE("mulf");
            auto op1 = reinterpret<f64>(stack_.back());
            stack_.pop_back();
            auto op2 = reinterpret<f64>(stack_.back());
            stack_.pop_back();
            stack_.push_back(reinterpret<u64>(op2 * op1));
            break;
        }
        case instruction::mulf_register_register:
        {
            PROFILE_SCOPE("mulf_register_register");
            Register* dest = to_register(*next_byte());
            Register* src = to_register(*next_byte());
            // add the two value as a signed integer then store as unsigned
            dest->x64 = reinterpret<u64>(reinterpret<f64>(dest->x64) * reinterpret<f64>(src->x64));
            break;
        }
        case instruction::divu:
        {
            PROFILE_SCOPE("divu");
            auto op1 = stack_.back();
            stack_.pop_back();
            auto op2 = stack_.back();
            stack_.pop_back();
            stack_.push_back(op2 / op1);
            break;
        }
        case instruction::divu_register_register:
        {
            PROFILE_SCOPE("divu_register_register");
            Register* dest = to_register(*next_byte());
            Register* src = to_register(*next_byte());
            dest->x64 = dest->x64 / src->x64;
            break;
        }
        case instruction::divs:
        {
            PROFILE_SCOPE("divs");
            auto op1 = reinterpret<i64>(stack_.back());
            stack_.pop_back();
            auto op2 = reinterpret<i64>(stack_.back());
            stack_.pop_back();
            stack_.push_back(reinterpret<u64>(op2 / op1));
            break;
        }
        case instruction::divs_register_register:
        {
            PROFILE_SCOPE("divs_register_register");
            Register* dest = to_register(*next_byte());
            Register* src = to_register(*next_byte());
            // add the two value as a signed integer then store as unsigned
            dest->x64 = reinterpret<u64>(reinterpret<i64>(dest->x64) / reinterpret<i64>(src->x64));
            break;
        }
        case instruction::divf:
        {
            PROFILE_SCOPE("divf");
            auto op1 = reinterpret<f64>(stack_.back());
            stack_.pop_back();
            auto op2 = reinterpret<f64>(stack_.back());
            stack_.pop_back();
            stack_.push_back(reinterpret<u64>(op2 / op1));
            break;
        }
        case instruction::divf_register_register:
        {
            PROFILE_SCOPE("divf_register_register");
            Register* dest = to_register(*next_byte());
            Register* src = to_register(*next_byte());
            // add the two value as a signed integer then store as unsigned
            dest->x64 = reinterpret<u64>(reinterpret<f64>(dest->x64) / reinterpret<f64>(src->x64));
            break;
        }
        case instruction::modu:
        {
            PROFILE_SCOPE("modu");
            auto op1 = stack_.back();
            stack_.pop_back();
            auto op2 = stack_.back();
            stack_.pop_back();
            stack_.push_back(op2 % op1);
            break;
        }
        case instruction::modu_register_register:
        {
            PROFILE_SCOPE("modu_register_register");
            Register* dest = to_register(*next_byte());
            Register* src = to_register(*next_byte());
            dest->x64 = dest->x64 % src->x64;
            break;
        }
        case instruction::mods:
        {
            PROFILE_SCOPE("mods");
            auto op1 = reinterpret<i64>(stack_.back());
            stack_.pop_back();
            auto op2 = reinterpret<i64>(stack_.back());
            stack_.pop_back();
            stack_.push_back(reinterpret<u64>(op2 % op1));
            break;
        }
        case instruction::mods_register_register:
        {
            PROFILE_SCOPE("mods_register_register");
            Register* dest = to_register(*next_byte());
            Register* src = to_register(*next_byte());
            // add the two value as a signed integer then store as unsigned
            dest->x64 = reinterpret<u64>(reinterpret<i64>(dest->x64) % reinterpret<i64>(src->x64));
            break;
        }
        case instruction::andu:
        {
            PROFILE_SCOPE("andu");
            auto op1 = stack_.back();
            stack_.pop_back();
            auto op2 = stack_.back();
            stack_.pop_back();
            stack_.push_back(op2 & op1);
            break;
        }
        case instruction::andu_register_register:
        {
            PROFILE_SCOPE("andu_register_register");
            Register* dest = to_register(*next_byte());
            Register* src = to_register(*next_byte());
            dest->x64 = dest->x64 & src->x64;
            break;
        }
        case instruction::ands:
        {
            PROFILE_SCOPE("ands");
            auto op1 = reinterpret<i64>(stack_.back());
            stack_.pop_back();
            auto op2 = reinterpret<i64>(stack_.back());
            stack_.pop_back();
            stack_.push_back(reinterpret<u64>(op2 & op1));
            break;
        }
        case instruction::ands_register_register:
        {
            PROFILE_SCOPE("ands_register_register");
            Register* dest = to_register(*next_byte());
            Register* src = to_register(*next_byte());
            // add the two value as a signed integer then store as unsigned
            dest->x64 = reinterpret<u64>(reinterpret<i64>(dest->x64) & reinterpret<i64>(src->x64));
            break;
        }
        case instruction::oru:
        {
            PROFILE_SCOPE("oru");
            auto op1 = stack_.back();
            stack_.pop_back();
            auto op2 = stack_.back();
            stack_.pop_back();
            stack_.push_back(op2 | op1);
            break;
        }
        case instruction::oru_register_register:
        {
            PROFILE_SCOPE("oru_register_register");
            Register* dest = to_register(*next_byte());
            Register* src = to_register(*next_byte());
            dest->x64 = dest->x64 | src->x64;
            break;
        }
        case instruction::ors:
        {
            PROFILE_SCOPE("ors");
            auto op1 = reinterpret<i64>(stack_.back());
            stack_.pop_back();
            auto op2 = reinterpret<i64>(stack_.back());
            stack_.pop_back();
            stack_.push_back(reinterpret<u64>(op2 | op1));
            break;
        }
        case instruction::ors_register_register:
        {
            PROFILE_SCOPE("ors_register_register");
            Register* dest = to_register(*next_byte());
            Register* src = to_register(*next_byte());
            // add the two value as a signed integer then store as unsigned
            dest->x64 = reinterpret<u64>(reinterpret<i64>(dest->x64) | reinterpret<i64>(src->x64));
            break;
        }
        case instruction::equalu:
        {
            PROFILE_SCOPE("equalu");
            auto op1 = stack_.back();
            stack_.pop_back();
            auto op2 = stack_.back();
            stack_.pop_back();
            stack_.push_back(op2 == op1);
            break;
        }
        case instruction::equalu_register_register:
        {
            PROFILE_SCOPE("equalu_register_register");
            Register* dest = to_register(*next_byte());
            Register* src = to_register(*next_byte());
            stack_.push_back(dest->x64 == src->x64);
            break;
        }
        case instruction::equals:
        {
            PROFILE_SCOPE("equals");
            auto op1 = reinterpret<i64>(stack_.back());
            stack_.pop_back();
            auto op2 = reinterpret<i64>(stack_.back());
            stack_.pop_back();
            stack_.push_back(reinterpret<u64>(op2 == op1));
            break;
        }
        case instruction::equals_register_register:
        {
            PROFILE_SCOPE("equals_register_register");
            Register* dest = to_register(*next_byte());
            Register* src = to_register(*next_byte());
            // add the two value as a signed integer then store as unsigned
            stack_.push_back(reinterpret<u64>(reinterpret<i64>(dest->x64) == reinterpret<i64>(src->x64)));
            break;
        }
        case instruction::equalf:
        {
            PROFILE_SCOPE("equalf");
            auto op1 = reinterpret<f64>(stack_.back());
            stack_.pop_back();
            auto op2 = reinterpret<f64>(stack_.back());
            stack_.pop_back();
            stack_.push_back(reinterpret<u64>(op2 == op1));
            break;
        }
        case instruction::equalf_register_register:
        {
            PROFILE_SCOPE("equalf_register_register");
            Register* dest = to_register(*next_byte());
            Register* src = to_register(*next_byte());
            // add the two value as a signed integer then store as unsigned
            stack_.push_back(reinterpret<u64>(reinterpret<f64>(dest->x64) == reinterpret<f64>(src->x64)));
            break;
        }
        case instruction::not_equalu:
        {
            PROFILE_SCOPE("not_equalu");
            auto op1 = stack_.back();
            stack_.pop_back();
            auto op2 = stack_.back();
            stack_.pop_back();
            stack_.push_back(op2 != op1);
            break;
        }
        case instruction::not_equalu_register_register:
        {
            PROFILE_SCOPE("not_equalu_register_register");
            Register* dest = to_register(*next_byte());
            Register* src = to_register(*next_byte());
            stack_.push_back(dest->x64 != src->x64);
            break;
        }
        case instruction::not_equals:
        {
            PROFILE_SCOPE("not_equals");
            auto op1 = reinterpret<i64>(stack_.back());
            stack_.pop_back();
            auto op2 = reinterpret<i64>(stack_.back());
            stack_.pop_back();
            stack_.push_back(reinterpret<u64>(op2 != op1));
            break;
        }
        case instruction::not_equals_register_register:
        {
            PROFILE_SCOPE("not_equals_register_register");
            Register* dest = to_register(*next_byte());
            Register* src = to_register(*next_byte());
            // add the two value as a signed integer then store as unsigned
            stack_.push_back(reinterpret<u64>(reinterpret<i64>(dest->x64) != reinterpret<i64>(src->x64)));
            break;
        }
        case instruction::not_equalf:
        {
            PROFILE_SCOPE("not_equalf");
            auto op1 = reinterpret<f64>(stack_.back());
            stack_.pop_back();
            auto op2 = reinterpret<f64>(stack_.back());
            stack_.pop_back();
            stack_.push_back(reinterpret<u64>(op2 != op1));
            break;
        }
        case instruction::not_equalf_register_register:
        {
            PROFILE_SCOPE("not_equalf_register_register");
            Register* dest = to_register(*next_byte());
            Register* src = to_register(*next_byte());
            // add the two value as a signed integer then store as unsigned
            stack_.push_back(reinterpret<u64>(reinterpret<f64>(dest->x64) != reinterpret<f64>(src->x64)));
            break;
        }
        case instruction::gtu:
        {
            PROFILE_SCOPE("gtu");
            auto op1 = stack_.back();
            stack_.pop_back();
            auto op2 = stack_.back();
            stack_.pop_back();
            stack_.push_back(op2 > op1);
            break;
        }
        case instruction::gtu_register_register:
        {
            PROFILE_SCOPE("gtu_register_register");
            Register* dest = to_register(*next_byte());
            Register* src = to_register(*next_byte());
            stack_.push_back(dest->x64 > src->x64);
            break;
        }
        case instruction::gts:
        {
            PROFILE_SCOPE("gts");
            auto op1 = reinterpret<i64>(stack_.back());
            stack_.pop_back();
            auto op2 = reinterpret<i64>(stack_.back());
            stack_.pop_back();
            stack_.push_back(reinterpret<u64>(op2 > op1));
            break;
        }
        case instruction::gts_register_register:
        {
            PROFILE_SCOPE("gts_register_register");
            Register* dest = to_register(*next_byte());
            Register* src = to_register(*next_byte());
            // add the two value as a signed integer then store as unsigned
            stack_.push_back(reinterpret<u64>(reinterpret<i64>(dest->x64) > reinterpret<i64>(src->x64)));
            break;
        }
        case instruction::gtf:
        {
            PROFILE_SCOPE("gtf");
            auto op1 = reinterpret<f64>(stack_.back());
            stack_.pop_back();
            auto op2 = reinterpret<f64>(stack_.back());
            stack_.pop_back();
            stack_.push_back(reinterpret<u64>(op2 > op1));
            break;
        }
        case instruction::gtf_register_register:
        {
            PROFILE_SCOPE("gtf_register_register");
            Register* dest = to_register(*next_byte());
            Register* src = to_register(*next_byte());
            // add the two value as a signed integer then store as unsigned
            stack_.push_back(reinterpret<u64>(reinterpret<f64>(dest->x64) > reinterpret<f64>(src->x64)));
            break;
        }
        case instruction::gtequalu:
        {
            PROFILE_SCOPE("gtequalu");
            auto op1 = stack_.back();
            stack_.pop_back();
            auto op2 = stack_.back();
            stack_.pop_back();
            stack_.push_back(op2 >= op1);
            break;
        }
        case instruction::gtequalu_register_register:
        {
            PROFILE_SCOPE("gtequalu_register_register");
            Register* dest = to_register(*next_byte());
            Register* src = to_register(*next_byte());
            stack_.push_back(dest->x64 >= src->x64);
            break;
        }
        case instruction::gtequals:
        {
            PROFILE_SCOPE("gtequals");
            auto op1 = reinterpret<i64>(stack_.back());
            stack_.pop_back();
            auto op2 = reinterpret<i64>(stack_.back());
            stack_.pop_back();
            stack_.push_back(reinterpret<u64>(op2 >= op1));
            break;
        }
        case instruction::gtequals_register_register:
        {
            PROFILE_SCOPE("gtequals_register_register");
            Register* dest = to_register(*next_byte());
            Register* src = to_register(*next_byte());
            // add the two value as a signed integer then store as unsigned
            stack_.push_back(reinterpret<u64>(reinterpret<i64>(dest->x64) >= reinterpret<i64>(src->x64)));
            break;
        }
        case instruction::gtequalf:
        {
            PROFILE_SCOPE("gtequalf");
            auto op1 = reinterpret<f64>(stack_.back());
            stack_.pop_back();
            auto op2 = reinterpret<f64>(stack_.back());
            stack_.pop_back();
            stack_.push_back(reinterpret<u64>(op2 >= op1));
            break;
        }
        case instruction::gtequalf_register_register:
        {
            PROFILE_SCOPE("gtequalf_register_register");
            Register* dest = to_register(*next_byte());
            Register* src = to_register(*next_byte());
            // add the two value as a signed integer then store as unsigned
            stack_.push_back(reinterpret<u64>(reinterpret<f64>(dest->x64) >= reinterpret<f64>(src->x64)));
            break;
        }
        case instruction::ltu:
        {
            PROFILE_SCOPE("ltu");
            auto op1 = stack_.back();
            stack_.pop_back();
            auto op2 = stack_.back();
            stack_.pop_back();
            stack_.push_back(op2 < op1);
            break;
        }
        case instruction::ltu_register_register:
        {
            PROFILE_SCOPE("ltu_register_register");
            Register* dest = to_register(*next_byte());
            Register* src = to_register(*next_byte());
            stack_.push_back(dest->x64 < src->x64);
            break;
        }
        case instruction::lts:
        {
            PROFILE_SCOPE("lts");
            auto op1 = reinterpret<i64>(stack_.back());
            stack_.pop_back();
            auto op2 = reinterpret<i64>(stack_.back());
            stack_.pop_back();
            stack_.push_back(reinterpret<u64>(op2 < op1));
            break;
        }
        case instruction::lts_register_register:
        {
            PROFILE_SCOPE("lts_register_register");
            Register* dest = to_register(*next_byte());
            Register* src = to_register(*next_byte());
            // add the two value as a signed integer then store as unsigned
            stack_.push_back(reinterpret<u64>(reinterpret<i64>(dest->x64) < reinterpret<i64>(src->x64)));
            break;
        }
        case instruction::ltf:
        {
            PROFILE_SCOPE("ltf");
            auto op1 = reinterpret<f64>(stack_.back());
            stack_.pop_back();
            auto op2 = reinterpret<f64>(stack_.back());
            stack_.pop_back();
            stack_.push_back(reinterpret<u64>(op2 < op1));
            break;
        }
        case instruction::ltf_register_register:
        {
            PROFILE_SCOPE("ltf_register_register");
            Register* dest = to_register(*next_byte());
            Register* src = to_register(*next_byte());
            // add the two value as a signed integer then store as unsigned
            stack_.push_back(reinterpret<u64>(reinterpret<f64>(dest->x64) < reinterpret<f64>(src->x64)));
            break;
        }
        case instruction::ltequalu:
        {
            PROFILE_SCOPE("ltequalu");
            auto op1 = stack_.back();
            stack_.pop_back();
            auto op2 = stack_.back();
            stack_.pop_back();
            stack_.push_back(op2 <= op1);
            break;
        }
        case instruction::ltequalu_register_register:
        {
            PROFILE_SCOPE("ltequalu_register_register");
            Register* dest = to_register(*next_byte());
            Register* src = to_register(*next_byte());
            stack_.push_back(dest->x64 <= src->x64);
            break;
        }
        case instruction::ltequals:
        {
            PROFILE_SCOPE("ltequals");
            auto op1 = reinterpret<i64>(stack_.back());
            stack_.pop_back();
            auto op2 = reinterpret<i64>(stack_.back());
            stack_.pop_back();
            stack_.push_back(reinterpret<u64>(op2 <= op1));
            break;
        }
        case instruction::ltequals_register_register:
        {
            PROFILE_SCOPE("ltequals_register_register");
            Register* dest = to_register(*next_byte());
            Register* src = to_register(*next_byte());
            // add the two value as a signed integer then store as unsigned
            stack_.push_back(reinterpret<u64>(reinterpret<i64>(dest->x64) <= reinterpret<i64>(src->x64)));
            break;
        }
        case instruction::ltequalf:
        {
            PROFILE_SCOPE("ltequalf");
            auto op1 = reinterpret<f64>(stack_.back());
            stack_.pop_back();
            auto op2 = reinterpret<f64>(stack_.back());
            stack_.pop_back();
            stack_.push_back(reinterpret<u64>(op2 <= op1));
            break;
        }
        case instruction::ltequalf_register_register:
        {
            PROFILE_SCOPE("ltequalf_register_register");
            Register* dest = to_register(*next_byte());
            Register* src = to_register(*next_byte());
            // compare floats
            stack_.push_back(reinterpret<u64>(reinterpret<f64>(dest->x64) <= reinterpret<f64>(src->x64)));
            break;
        }
        case instruction::push_string:
        {
            PROFILE_SCOPE("push_string");
            auto length = *reinterpret<u32*>(next_byte());
            program_counter_ += 3;
            auto *buffer = new char[length];
            memcpy_s(buffer, length, program_ + program_counter_ + 1, length);
            program_counter_ += length;
            stack_.push_back(reinterpret<u64>(new string(length, buffer)));
            break;
        }
        case instruction::free_string:
        {
            PROFILE_SCOPE("free_string");
            auto *str = reinterpret<string*>(stack_.back());
            delete str;
            break;
        }
        case instruction::string_len:
        {
            PROFILE_SCOPE("string_len");
            auto *str = reinterpret<string*>(stack_.back());
            stack_.pop_back();
            stack_.push_back(str->length);
            break;
        }
        case instruction::string_add:
        {
            PROFILE_SCOPE("string_add");
            auto* left = reinterpret<string*>(stack_.back());
            stack_.pop_back();
            auto* right = reinterpret<string*>(stack_.back());
            stack_.pop_back();
            stack_.push_back(reinterpret<u64>(*left + *right));
            delete left;
            delete right;
            break;
        }
        case instruction::jump_short:
        {
            PROFILE_SCOPE("jump_short");
            jump(reinterpret<u8>(*next_byte()));
            break;
        }
        case instruction::jump_long:
        {
            PROFILE_SCOPE("jump_long");
            jump(reinterpret<u8>(*reinterpret<u32*>(next_byte())));
            break;
        }
        case instruction::jump_ifzero:
        {
            PROFILE_SCOPE("jump_ifzero");
            if (stack_.back() == 0)
                jump(reinterpret<u8>(*next_byte()));
            else
                next_byte();
            break;
        }
        case instruction::jump_ifnzero:
        {
            PROFILE_SCOPE("jump_ifnzero");
            if (stack_.back() != 0)
                jump(reinterpret<u8>(*next_byte()));
            else
                next_byte();
            break;
        }
        case instruction::call_short:
        {
            PROFILE_SCOPE("call_short");
            auto num_of_params = reinterpret<u8>(*next_byte());
            auto jump_address = reinterpret<u8>(*next_byte());
            // number of params
            stack_.push_back(num_of_params);
            // old return address
            stack_.push_back(program_counter_ + 1);   // add one more to the program counter because we want jump to the next instruction when returning
            // old base pointer
            stack_.push_back(base_pointer_);
            // the new base pointer is equal to the current stack pointer
            base_pointer_ = stack_pointer();
            jump(jump_address);
            break;
        }
        case instruction::call_long:
        {
            PROFILE_SCOPE("call_long");
            auto num_of_params = reinterpret<u8>(*next_byte());
            auto jump_address = reinterpret<u32>(*next_byte()); // doesn't actually take 32 bits
            // program_counter_ += 2;
            // number of params
            stack_.push_back(num_of_params);
            // old return address
            stack_.push_back(program_counter_ + 1);   // add one more to the program counter because we want jump to the next instruction when returning
            // old base pointer
            stack_.push_back(base_pointer_);
            // the new base pointer is equal to the current stack pointer
            base_pointer_ = stack_pointer();
            jump(jump_address);
            break;
        }
        case instruction::load_arg:
        {
            PROFILE_SCOPE("load_arg");
            stack_.push_back(stack_[static_cast<u32>(base_pointer_ - (3LL + *next_byte()))]);
            break;
        }
        case instruction::dup:
        {
            PROFILE_SCOPE("dup");
            stack_.push_back(stack_.back());
            break;
        }
        case instruction::allocate_locals:
        {
            PROFILE_SCOPE("allocate_locals");
            auto locals = *next_byte();
            for (auto i = 0; i < locals; ++i)
                stack_.push_back(0);
            break;
        }
        case instruction::deallocate_locals:
        {
            PROFILE_SCOPE("deallocate_locals");
            auto locals = *next_byte();
            // for(auto i = 0; i < locals; ++i)
            //     stack_.pop_back();
            // break;

            stack_.erase(stack_.end() - locals - 1, stack_.end() - 1);
            break;
        }
        case instruction::set_local:
        {
            PROFILE_SCOPE("set_local");
            auto val = *next_byte();
            stack_[static_cast<u32>(base_pointer_ + val)] = stack_.back();
            stack_.pop_back();
            break;
        }
        case instruction::load_local:
        {
            PROFILE_SCOPE("load_local");
            auto val = *next_byte();
            stack_.push_back(stack_[static_cast<u32>(base_pointer_ + val)]);
            break;
        }
        default:
            break;
    }
}
