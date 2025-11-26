#pragma once
#include "bytecode.hpp"
#include "parser.hpp"

struct Program{
    char* data_section;
    int entry_point;
    InstructionList* instructions;
    Program(Expression* expr);
};