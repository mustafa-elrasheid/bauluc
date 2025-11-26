#include <vector>
#include <stddef.h>
#include <stdint.h>
#include <memory.h>
#include <stdio.h>
#include <stdint.h>
#include <dlfcn.h>

#include "bytecode.hpp"

union address_as_int{
    uint64_t value;
    uint64_t* address;
};

struct VirtualMachine{
    int _stack_size;
    int64_t* Registers;
    int64_t* Stack;
    inline int64_t parameter_value(Parameter* param);
    inline int64_t* destination(Parameter param);
    inline int64_t binary_operator(InstructionType type,int64_t value1,int64_t value2);
    VirtualMachine(int64_t entrypoint,char* data_Section,int64_t stack_size,int64_t registers_num);
    void Run(const std::vector<Instruction*>* instructions,bool info);
    ~VirtualMachine();
};
