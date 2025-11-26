#include "headers/program.hpp"

struct Label{
    int offset;
    const char* identifier;
};

struct LabelList{
    int length;
    int index;
    Label** labels;
    LabelList(){
        this->index = 0;
        this->length = 1000;
        this->labels = (Label**)malloc(sizeof(Label*)*1000);
    }
    void push(Label* label){
        labels[index] = label;
        index++;
    }
};

int Max(int x,int y){
	if(x > y)return x;
	return y;
}

void _Expression(Expression* expr, InstructionList* instructions){
    if(strcmp(expr->identifier,"Expression")!=0)return;
    const char* expr_type = expr->expressions->expressions[0]->identifier;
    if(strcmp(expr_type,"IDENTIFIER")==0){
        return;
    }
    if(strcmp(expr_type,"NUMBER")==0){
        instructions->push(new Instruction(InstructionType::PUSH,Parameter(RegisterType::Null,atoi(expr->expressions->expressions[0]->token->token))));
        return;
    }
    if(strcmp(expr_type,"Expression")==0){
        return;
    }
}

void Statement(Expression* expr, InstructionList* instructions, int* stack_size, LabelList* labels){
    if(strcmp(expr->identifier,"Statement")!=0)return;
    const char* expr_type = expr->expressions->expressions[0]->identifier;
    if(strcmp(expr_type,"RETURN")==0){
        _Expression(expr->expressions->expressions[1],instructions);
        instructions->push(new Instruction(POP, Parameter(AX, 0)));
        instructions->push(new Instruction(MOV, Parameter(SP, 0),Parameter(BP, 0)));
        instructions->push(new Instruction(POP, Parameter(BP, 0))); 
        instructions->push(new Instruction(POP, Parameter(BX, 0))); 
        instructions->push(new Instruction(JMP, Parameter(BX, 0))); 
        return;
    }
    if(strcmp(expr_type,"IF")==0){
        _Expression(expr->expressions->expressions[1],instructions);
        instructions->push(new Instruction(POP, Parameter(CR,0)));
        // skip if false by jumping ,continu if true by the command failing. the jump to line N is unknown
        Instruction* jmp_instruction = new Instruction(JN, Parameter(Null,0));
        instructions->push(jmp_instruction);
        Statement(expr->expressions->expressions[3],instructions,stack_size,labels);
        instructions->push(new Instruction(NOP));
        jmp_instruction->Parameters[0].offset = instructions->length - 1;
        return;
    }
    if(strcmp(expr_type,"WHILE")==0){
        instructions->push(new Instruction(NOP));
        int comparation_line =  instructions->length - 1;
        _Expression(expr->expressions->expressions[1],instructions);
        instructions->push(            new Instruction(POP, Parameter(CR,   0)));
        Instruction* jmp_instruction = new Instruction(JN , Parameter(Null, 0));
        instructions->push(jmp_instruction);
        Statement(expr->expressions->expressions[3],instructions,stack_size,labels);
        instructions->push(new Instruction(JMP, Parameter(Null, comparation_line)));
        instructions->push(new Instruction(NOP));
        jmp_instruction->Parameters[0].offset = instructions->length - 1;
        return;
    }
    if(strcmp(expr_type,"LET")==0){
        return;
    }
    if(strcmp(expr_type,"Expression")==0){
        _Expression(expr->expressions->expressions[0],instructions);
        instructions->push(new Instruction(POP, Parameter(AX, 0)));
        return;
    }
    if(strcmp(expr_type,"CURLY_BRACKET_OPEN")==0){
        int stack_frame_size = 0;
        for(int x = 1; x < expr->expressions->length-1; x++){
            int added_stack_frame = 0;
            int old_label_index = labels->index;
            Statement(expr->expressions->expressions[x],instructions,&added_stack_frame,labels);
            labels->index = old_label_index;
            stack_frame_size = Max(stack_frame_size,added_stack_frame);
        }
        *stack_size = stack_frame_size;
    }
}

void function(Expression* expr, InstructionList* instructions, int* entrypooint){
    if(strcmp(expr->identifier,"Function")!=0)return;
    bool is_main = strcmp(expr->expressions->expressions[0]->expressions->expressions[1]->token->token,"main" )== 0;
    instructions->push(new Instruction(NOP));
    if(is_main)
        *entrypooint = instructions->index;
    else{
        instructions->push(new Instruction(PUSH, Parameter(BP, 0)));
        instructions->push(new Instruction(MOV , Parameter(BP, 0), Parameter(SP, 0)));
    }
    // prealloc stack space
    Instruction* StackAllocter = new Instruction(SUB, Parameter(SP, 0), Parameter(Null, 0));
    instructions->push(StackAllocter);
    int stacksize = 0;
    Statement(expr->expressions->expressions[expr->expressions->length-1], instructions, &stacksize, new LabelList());    
    StackAllocter->Parameters[SECOND].offset = stacksize;
    if(is_main)
        instructions->push(new Instruction(EXIT));
    else{
        instructions->push(new Instruction(MOV, Parameter(SP, 0), Parameter(BP, 0)));
        instructions->push(new Instruction(POP, Parameter(BP, 0))); 
        instructions->push(new Instruction(POP, Parameter(BX, 0))); 
        instructions->push(new Instruction(JMP, Parameter(BX, 0))); 
    }
}

Program::Program(Expression* expr){
    this->entry_point = 0;
    this->instructions = new InstructionList();
    this->data_section = strdup("");
    if(strcmp(expr->identifier,"Functions")!=0)return;
    for(int x = 0; x < expr->expressions->length; x++){
        Expression* function_expr = expr->expressions->expressions[x];
        function(function_expr, this->instructions, &this->entry_point);
    }
}