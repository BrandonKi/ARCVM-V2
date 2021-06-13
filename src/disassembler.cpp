#include <iostream>
#include <string>

#include "Arcvm.h"

using instruction = Arcvm::instruction;

void disassemble(u8 *, size_t);

int main(int argc, char *argv[]) {
    auto *filename = argv[1];
    auto file = read_bin_file(filename);
    disassemble(file.buffer, file.size);
}

void disassemble(u8 *buffer, size_t size) {
    for (auto i = 0; i < size; ++i) {
        switch (buffer[i]) {
        case instruction::exit:
            std::cout << "exit\n";
            break;
        case instruction::ret:
            std::cout << "ret\n";
            break;
        case instruction::mov_register_value:
            std::cout << "mov_register_value\n";
            break;
        case instruction::mov_register_address:
            std::cout << "mov_register_address\n";
            break;
        case instruction::mov_register_register:
            std::cout << "mov_register_register\n";
            break;
        case instruction::mov_address_value:
            std::cout << "mov_address_value\n";
            break;
        case instruction::mov_address_address:
            std::cout << "mov_address_address\n";
            break;
        case instruction::mov_address_register:
            std::cout << "mov_address_register\n";
            break;
        case instruction::push_value:
            std::cout << "push_value\n";
            i += 8;
            break;
        case instruction::push_value_float_32:
            std::cout << "push_value_float_32\n";
            i += 4;
            break;
        case instruction::push_value_float_64:
            std::cout << "push_value_float_64\n";
            i += 8;
            break;
        case instruction::push_value_1:
            std::cout << "push_value_1\n";
            i += 1;
            break;
        case instruction::push_value_signed_8:
            std::cout << "push_value_signed_8\n";
            i += 1;
            break;
        case instruction::push_value_signed_16:
            std::cout << "push_value_signed_16\n";
            i += 1;
            break;
        case instruction::push_value_signed_32:
            std::cout << "push_value_signed_32\n";
            i += 4;
            break;
        case instruction::push_value_signed_64:
            std::cout << "push_value_signed_64\n";
            i += 8;
            break;
        case instruction::push_value_unsigned_8:
            std::cout << "push_value_unsigned_8\n";
            i += 1;
            break;
        case instruction::push_value_unsigned_16:
            std::cout << "push_value_unsigned_16\n";
            i += 1;
            break;
        case instruction::push_value_unsigned_32:
            std::cout << "push_value_unsigned_32\n";
            i += 4;
            break;
        case instruction::push_value_unsigned_64:
            std::cout << "push_value_unsigned_64\n";
            i += 8;
            break;
        case instruction::push_address:
            std::cout << "push_address\n";
            break;
        case instruction::push_register:
            std::cout << "push_register\n";
            break;
        case instruction::pop_register:
            std::cout << "pop_register\n";
            break;
        case instruction::pop_address:
            std::cout << "pop_address\n";
            break;
        case instruction::addu:
            std::cout << "addu\n";
            break;
        case instruction::addu_register_register:
            std::cout << "addu_register_register\n";
            break;
        case instruction::adds:
            std::cout << "adds\n";
            break;
        case instruction::adds_register_register:
            std::cout << "adds_register_register\n";
            break;
        case instruction::addf:
            std::cout << "addf\n";
            break;
        case instruction::addf_register_register:
            std::cout << "addf_register_register\n";
            break;
        case instruction::subu:
            std::cout << "subu\n";
            break;
        case instruction::subu_register_register:
            std::cout << "subu_register_register\n";
            break;
        case instruction::subs:
            std::cout << "subs\n";
            break;
        case instruction::subs_register_register:
            std::cout << "subs_register_register\n";
            break;
        case instruction::subf:
            std::cout << "subf\n";
            break;
        case instruction::subf_register_register:
            std::cout << "subf_register_register\n";
            break;
        case instruction::mulu:
            std::cout << "mulu\n";
            break;
        case instruction::mulu_register_register:
            std::cout << "mulu_register_register\n";
            break;
        case instruction::muls:
            std::cout << "muls\n";
            break;
        case instruction::muls_register_register:
            std::cout << "muls_register_register\n";
            break;
        case instruction::mulf:
            std::cout << "mulf\n";
            break;
        case instruction::mulf_register_register:
            std::cout << "mulf_register_register\n";
            break;
        case instruction::divu:
            std::cout << "divu\n";
            break;
        case instruction::divu_register_register:
            std::cout << "divu_register_register\n";
            break;
        case instruction::divs:
            std::cout << "divs\n";
            break;
        case instruction::divs_register_register:
            std::cout << "divs_register_register\n";
            break;
        case instruction::divf:
            std::cout << "divf\n";
            break;
        case instruction::divf_register_register:
            std::cout << "divf_register_register\n";
            break;
        case instruction::modu:
            std::cout << "modu\n";
            break;
        case instruction::modu_register_register:
            std::cout << "modu_register_register\n";
            break;
        case instruction::mods:
            std::cout << "mods\n";
            break;
        case instruction::mods_register_register:
            std::cout << "mods_register_register\n";
            break;
        case instruction::andu:
            std::cout << "andu\n";
            break;
        case instruction::andu_register_register:
            std::cout << "andu_register_register\n";
            break;
        case instruction::ands:
            std::cout << "ands\n";
            break;
        case instruction::ands_register_register:
            std::cout << "ands_register_register\n";
            break;
        case instruction::oru:
            std::cout << "oru\n";
            break;
        case instruction::oru_register_register:
            std::cout << "oru_register_register\n";
            break;
        case instruction::ors:
            std::cout << "ors\n";
            break;
        case instruction::ors_register_register:
            std::cout << "ors_register_register\n";
            break;
        case instruction::equalu:
            std::cout << "equalu\n";
            break;
        case instruction::equalu_register_register:
            std::cout << "equalu_register_register\n";
            break;
        case instruction::equals:
            std::cout << "equals\n";
            break;
        case instruction::equals_register_register:
            std::cout << "equals_register_register\n";
            break;
        case instruction::equalf:
            std::cout << "equalf\n";
            break;
        case instruction::equalf_register_register:
            std::cout << "equalf_register_register\n";
            break;
        case instruction::not_equalu:
            std::cout << "not_equalu\n";
            break;
        case instruction::not_equalu_register_register:
            std::cout << "not_equalu_register_register\n";
            break;
        case instruction::not_equals:
            std::cout << "not_equals\n";
            break;
        case instruction::not_equals_register_register:
            std::cout << "not_equals_register_register\n";
            break;
        case instruction::not_equalf:
            std::cout << "not_equalf\n";
            break;
        case instruction::not_equalf_register_register:
            std::cout << "not_equalf_register_register\n";
            break;
        case instruction::gtu:
            std::cout << "gtu\n";
            break;
        case instruction::gtu_register_register:
            std::cout << "gtu_register_register\n";
            break;
        case instruction::gts:
            std::cout << "gts\n";
            break;
        case instruction::gts_register_register:
            std::cout << "gts_register_register\n";
            break;
        case instruction::gtf:
            std::cout << "gtf\n";
            break;
        case instruction::gtf_register_register:
            std::cout << "gtf_register_register\n";
            break;
        case instruction::gtequalu:
            std::cout << "gtequalu\n";
            break;
        case instruction::gtequalu_register_register:
            std::cout << "gtequalu_register_register\n";
            break;
        case instruction::gtequals:
            std::cout << "gtequals\n";
            break;
        case instruction::gtequals_register_register:
            std::cout << "gtequals_register_register\n";
            break;
        case instruction::gtequalf:
            std::cout << "gtequalf\n";
            break;
        case instruction::gtequalf_register_register:
            std::cout << "gtequalf_register_register\n";
            break;
        case instruction::ltu:
            std::cout << "ltu\n";
            break;
        case instruction::ltu_register_register:
            std::cout << "ltu_register_register\n";
            break;
        case instruction::lts:
            std::cout << "lts\n";
            break;
        case instruction::lts_register_register:
            std::cout << "lts_register_register\n";
            break;
        case instruction::ltf:
            std::cout << "ltf\n";
            break;
        case instruction::ltf_register_register:
            std::cout << "ltf_register_register\n";
            break;
        case instruction::ltequalu:
            std::cout << "ltequalu\n";
            break;
        case instruction::ltequalu_register_register:
            std::cout << "ltequalu_register_register\n";
            break;
        case instruction::ltequals:
            std::cout << "ltequals\n";
            break;
        case instruction::ltequals_register_register:
            std::cout << "ltequals_register_register\n";
            break;
        case instruction::ltequalf:
            std::cout << "ltequalf\n";
            break;
        case instruction::ltequalf_register_register:
            std::cout << "ltequalf_register_register\n";
            break;
        case instruction::push_string:
            std::cout << "push_string\n";
            break;
        case instruction::free_string:
            std::cout << "free_string\n";
            break;
        case instruction::string_len:
            std::cout << "string_len\n";
            break;
        case instruction::string_add:
            std::cout << "string_add\n";
            break;
        case instruction::jump_short:
            std::cout << "jump_short\n";
            break;
        case instruction::jump_long:
            std::cout << "jump_long\n";
            break;
        case instruction::jump_ifzero:
            std::cout << "jump_ifzero\n";
            break;
        case instruction::jump_ifnzero:
            std::cout << "jump_ifnzero\n";
            break;
        case instruction::call_short:
            std::cout << "call_short\n";
            i += 2;
            break;
        case instruction::call_long:
            std::cout << "call_long\n";
            i += 4;
            break;
        case instruction::load_arg:
            std::cout << "load_arg\n";
            i += 1;
            break;
        case instruction::allocate_locals:
            std::cout << "allocate_locals\n";
            i += 1;
            break;
        case instruction::deallocate_locals:
            std::cout << "deallocate_locals\n";
            i += 1;
            break;
        case instruction::set_local:
            std::cout << "set_local\n";
            i += 1;
            break;
        case instruction::load_local:
            std::cout << "load_local\n";
            i += 1;
            break;
        case instruction::dup:
            std::cout << "dup\n";
            break;
        }
    }
}