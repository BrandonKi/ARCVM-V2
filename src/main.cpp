#include <iostream>
#include <fstream>
#include <sstream>

#include "Arcvm.h"

// TODO for now this just returns the filename but in the future this will actually parse the arguments and pass them into the VM 
std::string parseArgs(int, char**);

int main(int argc, char** argv) {
    std::string filename = parseArgs(argc, argv);

    // read the file into a u8 buffer
    // there is probably a more efficient way to do this though
    std::ifstream input_file(filename, std::ios::in|std::ios::binary|std::ios::ate);
    u32 size = static_cast<u32>(input_file.tellg());
    input_file.seekg (0, std::ios::beg);
    char* buffer = new char[size];
    input_file.read(buffer, size);
    input_file.close();
    Arcvm vm;
    if(vm.loadProgram(buffer, size) == false)  // will need to pass args in some way also
        std::cout << "ERROR FILE FORMAT UNSUPPORTED" << std::endl;
    else
        vm.run();
}

std::string parseArgs(int argc, char** argv) {
    // TODO this is not complete at all
    // at the moment it expects one argument that is a valid file path
    std::vector<std::string> args(argv, argv + argc);
    for(const std::string& arg : args) {
        if(arg == "TEMPORARY OPTION REPLACE MEEEEEE") {

        }
    }
    return args.back();
}
