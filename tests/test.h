#include "Arcvm.h"

#include <vector>

#define vm Arcvm::instruction

static i32 expected_result;

void expect(i32 result) {
    expected_result = result;
}

i32 runTest(std::vector<char> program) {
    Arcvm VM;
    VM.loadProgram(program.data(), program.size());
    return VM.run() == expected_result ? 0 : 1;
}