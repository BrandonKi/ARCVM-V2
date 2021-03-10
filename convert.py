#print('\n'.join(c.strip() for c in """                exit, ret, mov_register_value, mov_register_address, 
#                mov_register_register, mov_address_value, mov_address_address, mov_address_register, 
#                push_value, push_value_float_32, push_value_float_64, push_value_1, push_value_signed_8,
#                push_value_signed_16, push_value_signed_32, push_value_signed_64,
#                push_value_unsigned_8, push_value_unsigned_16, push_value_unsigned_32, push_value_unsigned_64,
#                push_address, push_register, pop_register, pop_address,
#                addu, addu_register_register, adds, adds_register_register, subu, subu_register_register,
#                subs, subs_register_register, mulu, mulu_register_register, muls, muls_register_register,
#                divu, divu_register_register, divs, divs_register_register, modu, modu_register_register,
#                mods, mods_register_register, andu, andu_register_register, ands, ands_register_register,
#                oru, oru_register_register, ors, ors_register_register, 
#                equalu, equalu_register_register, equals, equals_register_register,
#                not_equalu, not_equalu_register_register, not_equals, not_equals_register_register,
#                gtu, gtu_register_register, gts, gts_register_register, 
#                gtequalu, gtequalu_register_register, gtequals, gtequals_register_register, 
#                ltu, ltu_register_register, lts, lts_register_register, 
#                ltequalu, ltequalu_register_register, ltequals, ltequals_register_register, 
#                jump_short, jump_long, jump_ifzero, jump_ifnzero, call_short, call_long,
#                push_param,
#""".split(',')))

def main():
    with open("include/ARCVM.h") as f:
        content = f.read() 
        start = content.find('enum instruction')
        result = " ";        

        count = 0
        while result[-1] != '}':
            result += content[start + count]

        print(result)
if __name__ == "__main__":
    main()
