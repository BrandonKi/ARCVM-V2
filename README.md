# ARCVM-V2

The second version of ARCVM with a new instruction set

ARCVM is a mix between a stack-based and register-based virtual machine.
I supports operations that can be done directly on stack as well as operations between registers.
Internally all types are stored as 64 bits and it is up to the user to respect the type system and use correct operations according to type.

## Build/Usage
Uses the Ninja build system as an example
```
git clone https://github.com/BrandonKi/ARCVM-V2.git
cd ARCVM-V2
mkdir build
cd build
cmake .. -G Ninja
ninja ARCVM-V2
cd ../bin
./ARCVM-V2 <filename>
```

TODO list
* heap
* finish arrays
* calling C functions
* C FFI

## Instruction set (not complete)
https://docs.google.com/spreadsheets/d/1V8GoZfcoGQe3Bfy1f2PI5_oxU-PyuHK-nnY0LNCYBMo/edit?usp=sharing