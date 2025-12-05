#pragma once
#include "bytecode.hpp"
#include "../frontend/parser.hpp"

struct Label{
    int offset;
    const char* identifier;
    Label(const char* identif, int _offset);
};

struct LabelList{
    int length;
    int index;
    Label** labels;
    LabelList(int size = 1000);
    void push(Label* label);
    int find(const char* identifier);
    int peak();
};

struct Program{
    char* data_section;
    int entry_point;
    InstructionList* instructions;
    Program(Expression* expr);
    ~Program();
};