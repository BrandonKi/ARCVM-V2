#ifndef ARCVM_HEADER_GUARD
#define ARCVM_HEADER_GUARD


/** Expected file format
 * 
 * signature
 * 
 * 0xff 0xff
 * label table start
 * num of label table elements
 * label table data
 * 
 * init section
 * code
 * 
 */

/** Stack frame layout
 * 
 * parameters
 * 
 * return address
 * 
 * saved base pointer   <-- base pointer
 * 
 * locals
 * ...      <-- stack pointer
 * 
 */


#include <cstdint>
#include <vector>
#include <iostream>

using i8  = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using f32 = float_t;
using f64 = double_t;

class Arcvm {

    public:
        union Register {
            u64 x64;
            u32 x32;
            u16 x16;
            u8  x8;
        };

        enum instruction {
                exit, ret, mov_register_value, mov_register_address, 
                mov_register_register, mov_address_value, mov_address_address, mov_address_register, 
                push_value, push_address, push_register, pop_register, pop_address,
                addu, addu_register_register, adds, adds_register_register, subu, subu_register_register,
                subs, subs_register_register, mulu, mulu_register_register, muls, muls_register_register,
                divu, divu_register_register, divs, divs_register_register, modu, modu_register_register,
                mods, mods_register_register, jump_short, jump_long, call
            };

        Arcvm();
        ~Arcvm();
        void loadProgram(char*, size_t);
        i32 run();

    private:
        u64 stack_pointer;
        u64 program_counter;
        u64 base_pointer;

        Register registers[16] = {0};
        std::vector<u64> stack;
        u8* heap;

        u8 *program;
        size_t size;

        i32 exit_code;

        void execute();
        
        /**
         * reinterpret data as a different type
         */
        template <typename T, typename U>
        constexpr inline T reinterpret(U data) const {
            T temp;
            memcpy_s(&temp, sizeof(T), &data, sizeof(U));
            return temp;
        }

        /**
         * return a pointer to the next byte in the program 
         * and increment the program counter accordingly
         */
        constexpr inline u8 *nextByte() {
            return &program[++program_counter];
        }

        /**
         * return the register represented by the given byte
         * 
         */
        constexpr inline Register toRegister(u8 reg_num) {
            return registers[reg_num];
        }

};

#endif

/*
push_value_float_32 
push_value_float_64 

push_value_signed_1
push_value_signed_8
push_value_signed_16
push_value_signed_32
push_value_signed_64

push_value_unsigned_1
push_value_unsigned_8
push_value_unsigned_16
push_value_unsigned_32
push_value_unsigned_64

*/