#pragma once
#include "bytecode.hpp"
#include "../frontend/parser.hpp"

struct Program{
    char* data_section;
    int entry_point;
    InstructionList* instructions;
    Program(Expression* expr);
    ~Program();
};