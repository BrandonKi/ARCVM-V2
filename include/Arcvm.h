#ifndef ARCVM_HEADER_GUARD
#define ARCVM_HEADER_GUARD

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

        Arcvm();
        ~Arcvm();
        void loadProgram(const u8*, size_t);
        i32 run();

    private:
        reg registers[16];
        std::vector<u64> stack;
        u8* heap;

        const u8 *program;
        size_t size;

        u8 *stack_pointer;
        u8 *program_counter;
        u8 *frame_pointer;


};

#endif