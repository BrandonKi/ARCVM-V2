#include "Arcvm.h"

#include <vector>

#define vm Arcvm::instruction

static i32 expected_result;

/**
 * the vm returns i32 so the only way to return meaningful values of other types 
 * is to reinterpret them as an i32 then return
 * 
 * this function does the reinterpreting 
 * so checking if something is an expected result of a different type is easier
 */
template <typename T>
void expect(T result) {
    expected_result = *reinterpret_cast<i32*>(&result);
}

i32 runTest(std::vector<char> program) {
    Arcvm VM;
    VM.loadProgram(program.data(), program.size());
    return VM.run() == expected_result ? 0 : 1;
}

i32 runTest(std::vector<u8> program) {
    Arcvm VM;
    VM.loadProgram(reinterpret_cast<char*>(program.data()), program.size());
    return VM.run() == expected_result ? 0 : 1;
}
