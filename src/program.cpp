#include "headers/program.hpp"

void function(Expression* expr, InstructionList* intructions, int* entrypooint){
    if(strcmp(expr->identifier,"Function")!=0)return;
    
}

Program::Program(Expression* expr){
    if(strcmp(expr->identifier,"Functions")!=0)return;
    for(int x = 0; x < expr->expressions->length; x++){
        Expression* function_expr = expr->expressions->expressions[x];
        function(function_expr,this->instructions, &this->entry_point);
    }
}