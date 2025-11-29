#include "headers/program.hpp"

struct Label{
    int offset;
    const char* identifier;
    Label(const char* identif, int _offset){
        this->identifier = identif;
        this->offset = _offset;
    }
};

struct LabelList{
    int length;
    int index;
    Label** labels;
    LabelList(){
        this->index = -1;
        this->length = 1000;
        this->labels = (Label**)malloc(sizeof(Label*)*1000);
    }
    void push(Label* label){
        this->index+=1;
        this->labels[index] = label;
    }
    int find(const char* identifier){
        for(int x = 0; x < this->index+1; x++){
            if(strcmp(identifier,this->labels[x]->identifier) == 0)return this->labels[x]->offset;
        }
        return -1;
    }
    int peak(){
        if(index == -1)return 0;
        return this->labels[index]->offset;
    }
};

int Max(int x,int y){
	if(x > y)return x;
	return y;
}

void _Expression(Expression* expr, InstructionList* instructions, LabelList* function_labels, LabelList* var_labels, int deference_num = 0){
    const char* expr_type = expr->expressions->expressions[0]->identifier;
    if(strcmp(expr_type,"IDENTIFIER")==0){
        instructions->push(new Instruction(PUSH,Parameter(BP,0))); // the value is already in the top of the stack
        instructions->push(new Instruction(PUSH,Parameter(Null,var_labels->find(expr->expressions->expressions[0]->token->token))));
        instructions->push(new Instruction(SUB));
        for(int x = 0; x < deference_num+1; x++)
            instructions->push(new Instruction(DRFRNC));
        return;
    }
    if(strcmp(expr_type,"ROUND_BRACKET_OPEN")==0){
        _Expression(expr->expressions->expressions[1],instructions,function_labels,var_labels);
        return;
    }
    if(strcmp(expr_type,"BIT_NOT")==0){
        _Expression(expr->expressions->expressions[1],instructions,function_labels,var_labels);
        instructions->push(new Instruction(NOT));
        return;
    }
    if(strcmp(expr_type,"LOGIC_NOT")==0){
        _Expression(expr->expressions->expressions[1],instructions,function_labels,var_labels);
        instructions->push(new Instruction(NOT));
        return;
    }
    if(strcmp(expr_type,"SUB")==0){
        instructions->push(new Instruction(PUSH, Parameter(Null, 0)));
        _Expression(expr->expressions->expressions[1],instructions,function_labels,var_labels);
        instructions->push(new Instruction(SUB));
        return;
    }
    if(strcmp(expr_type,"STAR")==0){
        _Expression(expr->expressions->expressions[1],instructions,function_labels,var_labels,deference_num-1);
        return;
    }
    if(strcmp(expr_type,"AMPERSAND")==0){
        _Expression(expr->expressions->expressions[1],instructions,function_labels,var_labels,deference_num+1);
        return;
    }
    if(strcmp(expr_type,"NUMBER")==0){
        instructions->push(new Instruction(PUSH,Parameter(Null,atoi(expr->expressions->expressions[0]->token->token))));
        return;
    }
    if(strcmp(expr_type,"Expression") == 0 && strcmp(expr->expressions->expressions[2]->identifier,"Expression") == 0 && expr->expressions->length == 3){
        const char* operation_type = expr->expressions->expressions[1]->identifier;
        _Expression(expr->expressions->expressions[0],instructions,function_labels,var_labels,-(strcmp(operation_type,"EQUAL") == 0));
        _Expression(expr->expressions->expressions[2],instructions,function_labels,var_labels);
        if(strcmp(operation_type,"MODULO") == 0)
            instructions->push(new Instruction(REM));
        if(strcmp(operation_type,"IS_EQUAL") == 0)
            instructions->push(new Instruction(CMP));
        if(strcmp(operation_type,"ISNT_EQUAL") == 0)
            instructions->push(new Instruction(ADD)); //
        if(strcmp(operation_type,"IS_GREATER") == 0)
            instructions->push(new Instruction(CMPB));
        if(strcmp(operation_type,"IS_SMALLER") == 0)
            instructions->push(new Instruction(ADD)); //
        if(strcmp(operation_type,"IS_GREATER_OR_EQUAL") == 0)
            instructions->push(new Instruction(ADD)); //
        if(strcmp(operation_type,"IS_SMALLER_OR_EQUAL") == 0)
            instructions->push(new Instruction(ADD)); //
        if(strcmp(expr_type,"LOGIC_AND") == 0)
            instructions->push(new Instruction(AND));
        if(strcmp(operation_type,"LOGIC_OR") == 0)
            instructions->push(new Instruction(OR));
        if(strcmp(operation_type,"AMPERSAND") == 0)
            instructions->push(new Instruction(AND));
        if(strcmp(expr_type,"BIT_OR") == 0)
            instructions->push(new Instruction(OR));
        if(strcmp(operation_type,"BIT_XOR") == 0)
            instructions->push(new Instruction(XOR));
        if(strcmp(operation_type,"CARET") == 0)
            instructions->push(new Instruction(XOR));
        if(strcmp(operation_type,"STAR") == 0)
            instructions->push(new Instruction(MUL));
        if(strcmp(operation_type,"DIV") == 0)
            instructions->push(new Instruction(DIV));
        if(strcmp(operation_type,"PLUS") == 0)
            instructions->push(new Instruction(ADD));
        if(strcmp(operation_type,"SUB") == 0)
            instructions->push(new Instruction(SUB));

        if(strcmp(operation_type,"DOT") == 0)
            instructions->push(new Instruction(ADD)); //
        if(strcmp(operation_type,"ARROW") == 0)
            instructions->push(new Instruction(ADD)); //
        if(strcmp(operation_type,"EQUAL") == 0)
            instructions->push(new Instruction(MOV));
        
        return;
    }
    if(strcmp(expr_type,"Expression") == 0 && strcmp(expr->expressions->expressions[1]->identifier,"ROUND_BRACKET_OPEN") == 0){
        Instruction* jmp_int = new Instruction(InstructionType::PUSH,Parameter(RegisterType::Null,0));
		instructions->push(jmp_int); 

		instructions->push(new Instruction(InstructionType::PUSH)); 

		_Expression(expr->expressions->expressions[2],instructions,function_labels,var_labels);
		instructions->push(new Instruction(InstructionType::POP, Parameter(AX, 0)));
		
		instructions->push(new Instruction(InstructionType::POP, Parameter(AX, 0)));

		instructions->push(
			new Instruction(
				JMP,
				Parameter(
					Null,
					function_labels->find(expr->expressions->expressions[0]->expressions->expressions[0]->token->token)
		)));

		instructions->push(new Instruction(InstructionType::NOP)); 
		jmp_int->Parameters[FIRST].offset = instructions->index-1;

		instructions->push(new Instruction(InstructionType::PUSH,Parameter(RegisterType::AX,0)));
    }
}

void Statement(Expression* expr, InstructionList* instructions, int* stack_size, LabelList* function_labels, LabelList* var_labels){
    const char* expr_type = expr->expressions->expressions[0]->identifier;
    if(strcmp(expr_type,"RETURN")==0){
        _Expression(expr->expressions->expressions[1],instructions,function_labels,var_labels);
        instructions->push(new Instruction(POP, Parameter(AX, 0)));
        instructions->push(new Instruction(MOV, Parameter(SP, 0), Parameter(BP, 0)));
        instructions->push(new Instruction(POP, Parameter(BP, 0))); 
        instructions->push(new Instruction(POP, Parameter(BX, 0))); 
        instructions->push(new Instruction(JMP, Parameter(BX, 0))); 
        return;
    }
    if(strcmp(expr_type,"IF")==0){
        _Expression(expr->expressions->expressions[1],instructions,function_labels,var_labels);
        instructions->push(new Instruction(POP, Parameter(CR,0)));
        // skip if false by jumping ,continu if true by the command failing. the jump to line N is unknown
        Instruction* jmp_instruction = new Instruction(JN, Parameter(Null,0));
        instructions->push(jmp_instruction);
        Statement(expr->expressions->expressions[3],instructions,stack_size,function_labels,var_labels);
        instructions->push(new Instruction(NOP));
        jmp_instruction->Parameters[0].offset = instructions->index - 1;
        return;
    }
    if(strcmp(expr_type,"WHILE")==0){
        instructions->push(new Instruction(NOP));
        int comparation_line =  instructions->index - 1;
        _Expression(expr->expressions->expressions[1],instructions,function_labels,var_labels);
        instructions->push(            new Instruction(POP, Parameter(CR,   0)));
        Instruction* jmp_instruction = new Instruction(JN , Parameter(Null, 0));
        instructions->push(jmp_instruction);
        Statement(expr->expressions->expressions[3],instructions,stack_size,function_labels,var_labels);
        instructions->push(new Instruction(JMP, Parameter(Null, comparation_line)));
        instructions->push(new Instruction(NOP));
        jmp_instruction->Parameters[0].offset = instructions->index - 1;
        return;
    }
    if(strcmp(expr_type,"LET")==0){
        *stack_size += 8;
        var_labels->push(new Label(expr->expressions->expressions[2]->expressions->expressions[0]->expressions->expressions[0]->token->token,var_labels->peak()+8));
        _Expression(expr->expressions->expressions[2],instructions,function_labels,var_labels);
        instructions->push(new Instruction(POP, Parameter(AX, 0)));
        return;
    }
    if(strcmp(expr_type,"Expression")==0){
        _Expression(expr->expressions->expressions[0],instructions,function_labels,var_labels);
        instructions->push(new Instruction(POP, Parameter(AX, 0)));
        return;
    }
    if(strcmp(expr_type,"CURLY_BRACKET_OPEN")==0){
        Instruction* StackAllocter = new Instruction(SUB, Parameter(SP, 0), Parameter(Null, 0));
        instructions->push(StackAllocter);
        int statement_stack_size = 0;
        int old_label_index = var_labels->index;
        for(int x = 1; x < expr->expressions->length-1; x++){
            
            Statement(expr->expressions->expressions[x],instructions,&statement_stack_size,function_labels,var_labels);
        }
        var_labels->index = old_label_index;
        StackAllocter->Parameters[SECOND].offset = statement_stack_size;
    }
}

void function(Expression* expr, InstructionList* instructions, int* entrypoint, LabelList* function_labels){
    const char* function_name = expr->expressions->expressions[0]->expressions->expressions[1]->token->token;
    bool is_main = strcmp(function_name,"main" ) == 0;
    if(is_main) *entrypoint = instructions->index;
    function_labels->push(new Label(function_name,instructions->index));

    instructions->push(new Instruction(NOP));
    instructions->push(new Instruction(PUSH, Parameter(BP, 0)));
    instructions->push(new Instruction(MOV , Parameter(BP, 0), Parameter(SP, 0)));
    
    int stack_size = 8;
    LabelList* var_labels = new LabelList();
     // yah ik magic number
    if(expr->expressions->expressions[0]->expressions->length >= 7){
        var_labels->push(new Label(expr->expressions->expressions[0]->expressions->expressions[5]->token->token,8));
            instructions->push(new Instruction(SUB, Parameter(SP, 0), Parameter(Null, 8)));
        }
    Statement(expr->expressions->expressions[expr->expressions->length-1], instructions, &stack_size, function_labels, var_labels);    
    
    if(is_main)
        instructions->push(new Instruction(EXIT));
    else{
        instructions->push(new Instruction(MOV, Parameter(SP, 0), Parameter(BP, 0)));
        instructions->push(new Instruction(POP, Parameter(BP, 0))); 
        instructions->push(new Instruction(POP, Parameter(BX, 0))); 
        instructions->push(new Instruction(JMP, Parameter(BX, 0))); 
    };
}

Program::Program(Expression* expr){
    this->entry_point = 0;
    this->instructions = new InstructionList();
    this->data_section = strdup("");
    if(strcmp(expr->identifier,"Functions")!=0)return;
    LabelList* function_labels = new LabelList();
    for(int x = 0; x < expr->expressions->length; x++){
        Expression* function_expr = expr->expressions->expressions[x];
        function(function_expr, this->instructions, &this->entry_point, function_labels);
    }
}