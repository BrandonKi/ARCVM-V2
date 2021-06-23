#ifndef ARCVM_UTIL_H
#define ARCVM_UTIL_H

#include <fstream>

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

/**
 * reinterpret data as a different type
 */
template <typename T, typename U>
[[nodiscard]] constexpr inline T reinterpret(U data) {
    T temp;
    temp = std::bit_cast<T>(data);
    // needed for old c++ versions
    // static_assert(sizeof(T) <= sizeof(U), "The size of the result type must be <= the size of the parameter");
    // memcpy_s(&temp, sizeof(T), &data, sizeof(U));
    return temp;
}

struct read_bin_file_return_type {
    u8* buffer;
    size_t size;
};

/**
 * heap allocates a buffer to store the data
 */
[[nodiscard]] inline read_bin_file_return_type read_bin_file(char* filename) {
    std::ifstream input_file(filename, std::ios::in|std::ios::binary|std::ios::ate);
    const auto size = static_cast<u32>(input_file.tellg());
    input_file.seekg (0, std::ios::beg);
    auto* buffer = new char[size];
    input_file.read(buffer, size);
    input_file.close();
    // still ub
    // copy/reinterpret the whole buffer to avoid it
    return read_bin_file_return_type{reinterpret<u8*>(buffer), size};
}

#endif