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
 * code
 * 
 */


#include <cstdint>
#include <vector>

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
        union reg {
            u64 x64;
            u32 x32;
            u16 x16;
            u8  x8;
        };

        enum instruction {
                ret, mov_register_value, mov_register_address, 
                mov_register_register, mov_address_value, mov_address_address, mov_address_register, 
                push_value, push_address, push_register, pop_register, pop_address,
                add, add_register_register, sub, sub_register_register, mul, mul_register_register,
                div, div_register_register, mod, mod_register_register, jump, call
            };

        Arcvm();
        ~Arcvm();
        void loadProgram(const u8*, size_t);
        i32 run();
        void execute();

    private:
        reg registers[16];
        std::vector<u64> stack;
        u8* heap;

        const u8 *program;
        size_t size;

        u64 stack_pointer;
        u64 program_counter;
        u64 frame_pointer;


};

#endif