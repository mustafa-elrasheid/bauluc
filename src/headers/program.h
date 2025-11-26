#pragma once
#include "bytecode.h"
#include "parser.h"

struct Program{
    char* data_section;
    int entry_point;
    InstructionList* instructions;
    Program(Expression* expr);
};