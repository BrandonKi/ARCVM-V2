#include "test.h"

/**
 * push two signed 64 bit values to the stack and add them
 */
int main(int, char**) {

    std::vector<u8> program = {
        vm::push_value,
        0xf0,
        0xff,
        0xff,
        0xff,
        0xff,
        0xff,
        0xff,
        0xff,
        vm::push_value,
        0xf0,
        0xff,
        0xff,
        0xff,
        0xff,
        0xff,
        0xff,
        0xff,
        vm::adds,
        vm::ret
    };
    expect(-32);
    return runTest(program);
}