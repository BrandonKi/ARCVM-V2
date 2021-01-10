#include <iostream>
#include <fstream>
#include <sstream>

#include "Arcvm.h"

// for now this just returns the filename but in the future this will actually parse the arguments and pass them into the VM 
std::string parseArgs(int, char**);

int main(int argc, char** argv) {
    std::string filename = parseArgs(argc, argv);

    // read the file into a u8 buffer
    // there is probably a more efficient way to do this though
    std::ifstream input_stream(filename);
    std::stringstream buffer;
    buffer << input_stream.rdbuf();
    const u8 *data = reinterpret_cast<const u8*>(buffer.str().c_str());

    Arcvm vm;
    vm.loadProgram(data, buffer.str().size());  // will need to pass args in some way also
    vm.run();
}

std::string parseArgs(int argc, char** argv) {
    std::vector<std::string> args(argv, argv + argc);
    for(const std::string& arg : args) {
        if(arg == "TEMPORARY OPTION REPLACE MEEEEEE") {

        }
    }
    return args.back();
}
