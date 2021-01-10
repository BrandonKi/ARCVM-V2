#include "Arcvm.h"

Arcvm::Arcvm() {
    heap = (u8*)malloc(100000);
}

Arcvm::~Arcvm() {
    free(heap);
}

void Arcvm::loadProgram(const u8 *program, size_t size) {
    this->program = program;
    this->size = size;
}

i32 Arcvm::run() {
    // fetch
    // decode
    // execute
    return 0;
}
