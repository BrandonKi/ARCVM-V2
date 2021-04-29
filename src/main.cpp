#include <iostream>
#include <fstream>

#include "Arcvm.h"

// TODO for now this just returns the filename but in the future this will actually parse the arguments and pass them into the VM 
std::string parse_args(const int, const char**);

int main(const int argc, const char** argv) {
    PROFILE();
    const auto filename = parse_args(argc, argv);

    // read the file into a u8 buffer
    // there is probably a more efficient way to do this though
    std::ifstream input_file(filename, std::ios::in|std::ios::binary|std::ios::ate);
    const auto size = static_cast<u32>(input_file.tellg());
    input_file.seekg (0, std::ios::beg);
    auto* buffer = new char[size];  // no reason to delete
    input_file.read(buffer, size);
    input_file.close();
    Arcvm vm;
    if(!vm.load_program(buffer, size))  // will need to pass args in some way also
        std::cout << "ERROR FILE FORMAT UNSUPPORTED" << std::endl;
    else
        return vm.run();
}

std::string parse_args(const int argc, const char** argv) {
    PROFILE();
    // TODO this is not complete at all
    // at the moment it expects one argument that is a valid file path
    std::vector<std::string> args(argv, argv + argc);
    for(const auto& arg : args) {
        if(arg == "TEMPORARY OPTION REPLACE MEEEEEE") {

        }
    }
    return args.back();
}
