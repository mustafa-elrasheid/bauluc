#pragma once
#include "bytecode.hpp"
#include "../frontend/parser.hpp"

struct Label{
    int offset;
    const char* identifier;
    Label(const char* identif, int _offset);
    int** updated_offsets;
};

struct LabelList{
    bool allow_hoisting;
    int length;
    int index;
    Label** labels;
    LabelList(int size = 1000, bool allow_hoisting = false);
    void push(Label* label);
    int find(const char* identifier);
    void empty();
    int peak();
    ~LabelList();
};

struct Program{
    char* data_section;
    int entry_point;
    InstructionList* instructions;
    LabelList* var_labels;
    LabelList* function_labels;
    void parse_function(Expression* expr);
    void parse_statement(Expression* expr, int* stack_size);
    void parse_expression(Expression* expr, int deference_num);
    Program(Expression* expr);
    ~Program();
};