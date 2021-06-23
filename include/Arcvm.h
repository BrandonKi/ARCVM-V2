#ifndef ARCVM_HEADER_GUARD
#define ARCVM_HEADER_GUARD


/** Expected file format
 * 
 * signature
 * 
 * init section
 * code
 * 
 */

/** Stack frame layout
 * 
 * parameters
 * 
 * number of parameters
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
#include <memory>

#include <small_profiler.h>

#include "util.h"

class Arcvm {

    public:
        union Register {
            u64 x64;
            u32 x32;
            u16 x16;
            u8  x8;
        };

        enum instruction : u8 {
            exit,
            ret,

            mov_register_value,
            mov_register_address, 
            mov_register_register,
            mov_address_value,
            mov_address_address,
            mov_address_register,

            push_value,
            push_value_float_32,
            push_value_float_64,
            push_value_1,
            push_value_signed_8,
            push_value_signed_16,
            push_value_signed_32,
            push_value_signed_64,
            push_value_unsigned_8,
            push_value_unsigned_16,
            push_value_unsigned_32,
            push_value_unsigned_64,

            push_address,
            push_register,
            pop_register,
            pop_address,

            addu,
            addu_register_register,
            adds,
            adds_register_register,
            addf,
            addf_register_register,

            subu,
            subu_register_register,
            subs,
            subs_register_register,
            subf,
            subf_register_register,

            mulu,
            mulu_register_register,
            muls,
            muls_register_register,
            mulf,
            mulf_register_register,

            divu,
            divu_register_register,
            divs,
            divs_register_register,
            divf,
            divf_register_register,

            modu,
            modu_register_register,
            mods,
            mods_register_register,

            andu,
            andu_register_register,
            ands,
            ands_register_register,

            oru,
            oru_register_register,
            ors,
            ors_register_register,

            equalu,
            equalu_register_register,
            equals,
            equals_register_register,
            equalf,
            equalf_register_register,

            not_equalu,
            not_equalu_register_register,
            not_equals,
            not_equals_register_register,
            not_equalf,
            not_equalf_register_register,

            gtu,
            gtu_register_register,
            gts,
            gts_register_register,
            gtf,
            gtf_register_register,

            gtequalu,
            gtequalu_register_register,
            gtequals,
            gtequals_register_register,
            gtequalf,
            gtequalf_register_register,

            ltu,
            ltu_register_register,
            lts,
            lts_register_register,
            ltf,
            ltf_register_register,

            ltequalu,
            ltequalu_register_register,
            ltequals,
            ltequals_register_register,
            ltequalf,
            ltequalf_register_register,

            push_string,
            free_string,
            string_len,
            string_add,

            jump_short,
            jump_long,
            jump_ifzero,
            jump_ifnzero,

            call_short,
            call_long,

            load_arg,

            allocate_locals,
            deallocate_locals,
            set_local,
            load_local,

            dup,
        };

        struct string {
            char *data;
            u32 length;

            string() = default;

            string(const u32 length, char *data):
                data(data), length(length)
            {

            }

            ~string() {
                delete[] data;
            }

            string* operator + (const string& other) {
                auto* result = new char[this->length + other.length];
                memcpy_s(result, this->length, other.data, other.length);
                memcpy_s(result+other.length, this->length, other.data, other.length);
                return new string{ this->length + other.length, result };
            }
        };

        Arcvm();
        ~Arcvm();
        Arcvm(Arcvm&) = default;
        Arcvm(Arcvm&&) = default;
        Arcvm& operator = (const Arcvm&) = default;
        Arcvm& operator = (Arcvm&&) = default;
        bool load_program(char*, const size_t);
        i32 run();

    private:
        u64 program_counter_;
        u64 base_pointer_;

        Register registers_[16] = {{0}};
        std::vector<u64> stack_;
        u8* heap_;

        u8 *program_;
        size_t size_;

        i32 exit_code_;

        bool verifySignature();
        void execute();
        
        /**
         * reinterpret data as a different type
         */
        template <typename T, typename U>
        [[nodiscard]] constexpr inline T reinterpret(U data) const {
            static_assert(sizeof(T) <= sizeof(U), "The size of the result type must be <= the size of the parameter");
            T temp;
            memcpy_s(&temp, sizeof(T), &data, sizeof(U));
            return temp;
        }

        /**
         * return a pointer to the current byte in the program 
         */
        [[nodiscard]] constexpr inline u8 *current_byte() const noexcept {
            return &program_[program_counter_];
        }

        /**
         * return a pointer to the next byte in the program 
         * and increment the program counter accordingly
         */
        constexpr inline u8 *next_byte() noexcept {
            return &program_[++program_counter_];
        }

        /**
         * return the register represented by the given byte
         * 
         */
        [[nodiscard]] constexpr inline Register* to_register(const u8 reg_num) noexcept {
            return &registers_[reg_num];
        }

        /**
         * return the current stack pointer
         */
        [[nodiscard]] inline u64 stack_pointer() const noexcept {
            return stack_.size() - 1;
        }

        /**
         * jump to specified address
         */
        constexpr inline bool jump(const u8 address) noexcept {
            if(address >= size_) //TODO fix this asap it is just a temporary solution and it doesn't even work
                return false;
            // because the program counter is incremented after an instruction is executed
            // the jump will be off by one address so it is adjusted here to work as expected
            if(address == 0)
                program_counter_ = 0xffffffffffffffff;
            else
                program_counter_ = static_cast<u64>(address)-1;
            return true;
        }

        constexpr inline bool jump(const u32 address) noexcept {
            if(address >= size_)
                return false;
            if(address == 0)
                program_counter_ = 0xffffffffffffffff;
            else
                program_counter_ = static_cast<u64>(address-1);
            return true;
        }

        constexpr inline bool jump(const u64 address) noexcept {
            if(address >= size_)
                return false;
            if(address == 0)
                program_counter_ = 0xffffffffffffffff;
            else
                program_counter_ = address-1;
            return true;
        }

};

#endif