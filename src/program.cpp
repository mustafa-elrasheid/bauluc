#include "headers/program.hpp"

Label::Label(const char* identif, int _offset){
    this->identifier = identif;
    this->offset = _offset;
}

LabelList::LabelList(int size, bool allow_hoisting){
    this->index = -1;
    this->length = size;
    this->labels = (Label**)malloc(sizeof(Label*)*size);
    this->allow_hoisting = allow_hoisting;
}

void LabelList::push(Label* label){
    this->index += 1;
    this->labels[index] = label;
}

int LabelList::find(const char* identifier){
    for(int x = 0; x < this->index+1; x++){
        if(strcmp(identifier,this->labels[x]->identifier) == 0) return this->labels[x]->offset;
    }
    if(!allow_hoisting)
        error("Unknown Identifier","Identifier: \"%s\" was not found.\n",identifier);
    return -1;
}

int LabelList::peak(){
    if(index == -1) return 0;
    return this->labels[index]->offset;
}

void LabelList::empty(){
    for(int x = 0; x < this->index+1; x++)
        delete this->labels[x];
    this->index = -1;
}

LabelList::~LabelList(){
    for(int x = 0; x < this->index+1; x++)
        delete this->labels[x];
    free(labels);
}

int Max(int x,int y){
	if(x > y) return x;
	return y;
}

void Program::parse_expression(Expression* expr, int deference_num = 0){
    const char* expr_type = expr->expressions[0]->identifier;
    if(strcmp(expr_type,"IDENTIFIER")==0){
        instructions->push(new Instruction(PUSH, Parameter(BP, -var_labels->find(expr->expressions[0]->token->content))));
        for(int x = 0; x < deference_num+1; x++)
            instructions->push(new Instruction(DRFRNC));
        return;
    }
    if(strcmp(expr_type,"STRING")==0){
        instructions->push(new Instruction(PUSH, Parameter(data, strlen(this->data_section))));
        data_section = (char*) realloc(data_section,strlen(data_section)+strlen(expr->expressions[0]->token->content));
        strcat(data_section,expr->expressions[0]->token->content);
        return;
    }
    if(strcmp(expr_type,"ROUND_BRACKET_OPEN")==0){
        parse_expression(expr->expressions[1]);
        return;
    }
    if(strcmp(expr_type,"BIT_NOT")==0){
        parse_expression(expr->expressions[1]);
        instructions->push(new Instruction(NOT));
        return;
    }
    if(strcmp(expr_type,"LOGIC_NOT")==0){
        parse_expression(expr->expressions[1]);
        instructions->push(new Instruction(NOT));
        return;
    }
    if(strcmp(expr_type,"SUB")==0){
        instructions->push(new Instruction(PUSH, Parameter(Null, 0)));
        parse_expression(expr->expressions[1]);
        instructions->push(new Instruction(SUB));
        return;
    }
    if(strcmp(expr_type,"STAR")==0){
        parse_expression(expr->expressions[1],deference_num-1);
        return;
    }
    if(strcmp(expr_type,"AMPERSAND")==0){
        parse_expression(expr->expressions[1],deference_num+1);
        return;
    }
    if(strcmp(expr_type,"NUMBER")==0){
        instructions->push(new Instruction(PUSH,Parameter(Null,atoi(expr->expressions[0]->token->content))));
        return;
    }
    if(strcmp(expr_type,"Expression") == 0 && strcmp(expr->expressions[2]->identifier,"Expression") == 0 && expr->expressions.length == 3){
        const char* operation_type = expr->expressions[1]->identifier;
        if(strcmp(operation_type,"MODULO") == 0){
            parse_expression(expr->expressions[0]);
            parse_expression(expr->expressions[2]);
            instructions->push(new Instruction(REM));
            return;
        } 
        if(strcmp(operation_type,"IS_EQUAL") == 0){
            parse_expression(expr->expressions[0]);
            parse_expression(expr->expressions[2]);
            instructions->push(new Instruction(CMP));
            return;
        }
        if(strcmp(operation_type,"ISNT_EQUAL") == 0){
            parse_expression(expr->expressions[0]);
            parse_expression(expr->expressions[2]);
            instructions->push(new Instruction(CMP));
            instructions->push(new Instruction(NOT));
            return;
        }
        if(strcmp(operation_type,"IS_GREATER") == 0){
            parse_expression(expr->expressions[0]);
            parse_expression(expr->expressions[2]);
            instructions->push(new Instruction(CMPB));
            return;
        }
        if(strcmp(operation_type,"IS_SMALLER") == 0){
            parse_expression(expr->expressions[2]);
            parse_expression(expr->expressions[0]);
            instructions->push(new Instruction(CMPB)); //
            return;
        }
        if(strcmp(operation_type,"IS_GREATER_OR_EQUAL") == 0){
            parse_expression(expr->expressions[0]);
            parse_expression(expr->expressions[2]);
            instructions->push(new Instruction(CMPB));
            parse_expression(expr->expressions[0]);
            parse_expression(expr->expressions[2]);
            instructions->push(new Instruction(CMP));
            instructions->push(new Instruction(OR));
            return;
        }
        if(strcmp(operation_type,"IS_SMALLER_OR_EQUAL") == 0){
            parse_expression(expr->expressions[2]);
            parse_expression(expr->expressions[0]);
            instructions->push(new Instruction(CMPB));
            parse_expression(expr->expressions[0]);
            parse_expression(expr->expressions[2]);
            instructions->push(new Instruction(CMP));
            instructions->push(new Instruction(OR));
            return;
        }
        if(strcmp(expr_type,"LOGIC_AND") == 0){
            parse_expression(expr->expressions[0]);
            parse_expression(expr->expressions[2]);
            instructions->push(new Instruction(AND));
            return;
        }
        if(strcmp(operation_type,"LOGIC_OR") == 0){
            parse_expression(expr->expressions[0]);
            parse_expression(expr->expressions[2]);
            instructions->push(new Instruction(OR));
            return;
        }
        if(strcmp(operation_type,"AMPERSAND") == 0){
            parse_expression(expr->expressions[0]);
            parse_expression(expr->expressions[2]);
            instructions->push(new Instruction(AND));
            return;
        }
        if(strcmp(expr_type,"BIT_OR") == 0){
            parse_expression(expr->expressions[0]);
            parse_expression(expr->expressions[2]);
            instructions->push(new Instruction(OR));
            return;
        }
        if(strcmp(operation_type,"BIT_XOR") == 0){
            parse_expression(expr->expressions[0]);
            parse_expression(expr->expressions[2]);
            instructions->push(new Instruction(XOR));
            return;
        }
        if(strcmp(operation_type,"CARET") == 0){
            parse_expression(expr->expressions[0]);
            parse_expression(expr->expressions[2]);
            instructions->push(new Instruction(XOR));
            return;
        }
        if(strcmp(operation_type,"STAR") == 0){
            parse_expression(expr->expressions[0]);
            parse_expression(expr->expressions[2]);
            instructions->push(new Instruction(MUL));
            return;
        }
        if(strcmp(operation_type,"DIV") == 0){
            parse_expression(expr->expressions[0]);
            parse_expression(expr->expressions[2]);
            instructions->push(new Instruction(DIV));
            return;
        }
        if(strcmp(operation_type,"PLUS") == 0){
            parse_expression(expr->expressions[0]);
            parse_expression(expr->expressions[2]);
            instructions->push(new Instruction(ADD));
            return;
        }
        if(strcmp(operation_type,"SUB") == 0){
            parse_expression(expr->expressions[0]);
            parse_expression(expr->expressions[2]);
            instructions->push(new Instruction(SUB));
            return;
        }

        if(strcmp(operation_type,"DOT") == 0){
            throw "this feature is not yet implemented";
            return;
        }
        if(strcmp(operation_type,"ARROW") == 0){
            throw "this feature is not yet implemented";
            return;
        }
        if(strcmp(operation_type,"EQUAL") == 0){
            parse_expression(expr->expressions[0],-1);
            parse_expression(expr->expressions[2]);
            instructions->push(new Instruction(MOV));
            return;
        }
        return;
    }
    if(strcmp(expr_type,"Expression") == 0 && strcmp(expr->expressions[1]->identifier,"ROUND_BRACKET_OPEN") == 0){
        Instruction* jmp_int = new Instruction(PUSH,Parameter(Null,0));
		instructions->push(jmp_int); 

		instructions->push(new Instruction(PUSH,Parameter(Null,0)));
        if(expr->expressions.length > 3){
            for(int x = 0; x < expr->expressions.length-4; x++)
                parse_expression(expr->expressions[2+x]->expressions[0]);
            parse_expression(expr->expressions[expr->expressions.length-2]);
            for(int x = 0; x < expr->expressions.length-3; x++)
                instructions->push(new Instruction(POP, Parameter(AX, 0)));
        }
		
		instructions->push(new Instruction(POP, Parameter(AX, 0)));

		instructions->push(
			new Instruction(
				JMP,
				Parameter(
					Null,
					function_labels->find(expr->expressions[0]->expressions[0]->token->content)
		)));

		instructions->push(new Instruction(NOP)); 
		jmp_int->Parameters[FIRST].offset = instructions->index-1;

		instructions->push(new Instruction(PUSH,Parameter(AX,0)));
    }
}

void Program::parse_statement(Expression* expr, int* stack_size){
    const char* expr_type = expr->expressions[0]->identifier;
    if(strcmp(expr_type,"RETURN")==0){
        parse_expression(expr->expressions[1]);
        instructions->push(new Instruction(POP, Parameter(AX, 0)));
        instructions->push(new Instruction(MOV, Parameter(SP, 0), Parameter(BP, 0)));
        instructions->push(new Instruction(POP, Parameter(BP, 0))); 
        instructions->push(new Instruction(POP, Parameter(BX, 0))); 
        instructions->push(new Instruction(JMP, Parameter(BX, 0))); 
        return;
    }
    if(strcmp(expr_type,"IF")==0){
        parse_expression(expr->expressions[1]);
        instructions->push(new Instruction(POP, Parameter(CR,0)));
        // skip if false by jumping ,continu if true by the command failing. the jump to line N is unknown
        Instruction* jmp_instruction = new Instruction(JN, Parameter(Null,0));
        instructions->push(jmp_instruction);
        parse_statement(expr->expressions[3],stack_size);
        instructions->push(new Instruction(NOP));
        jmp_instruction->Parameters[0].offset = instructions->index - 1;
        return;
    }
    if(strcmp(expr_type,"WHILE")==0){
        instructions->push(new Instruction(NOP));
        int comparation_line =  instructions->index - 1;
        parse_expression(expr->expressions[1]);
        instructions->push(            new Instruction(POP, Parameter(CR,   0)));
        Instruction* jmp_instruction = new Instruction(JN , Parameter(Null, 0));
        instructions->push(jmp_instruction);
        parse_statement(expr->expressions[3],stack_size);
        instructions->push(new Instruction(JMP, Parameter(Null, comparation_line)));
        instructions->push(new Instruction(NOP));
        jmp_instruction->Parameters[0].offset = instructions->index - 1;
        return;
    }
    if(strcmp(expr_type,"LET")==0){
        *stack_size += 8;
        var_labels->push(new Label(expr->expressions[2]->expressions[0]->expressions[0]->token->content,var_labels->peak()+8));
        parse_expression(expr->expressions[2]);
        instructions->push(new Instruction(POP, Parameter(AX, 0)));
        return;
    }
    if(strcmp(expr_type,"Expression")==0){
        parse_expression(expr->expressions[0]);
        instructions->push(new Instruction(POP, Parameter(AX, 0)));
        return;
    }
    if(strcmp(expr_type,"CURLY_BRACKET_OPEN")==0){
        Instruction* StackAllocter = new Instruction(SUB, Parameter(SP, 0), Parameter(Null, 0));
        instructions->push(StackAllocter);
        int statement_stack_size = 0;
        int old_label_index = var_labels->index;
        for(int x = 1; x < expr->expressions.length-1; x++)
            parse_statement(expr->expressions[x],&statement_stack_size);
        var_labels->index = old_label_index;
        StackAllocter->Parameters[SECOND].offset = statement_stack_size;
        instructions->push(new Instruction(ADD, Parameter(SP, 0), Parameter(Null, statement_stack_size)));
    }
}

void Program::parse_function(Expression* expr){
    Expression* function_definition = expr->expressions[0];
    const char* function_name = function_definition->expressions[1]->token->content;
    bool is_start = strcmp(function_name,"_start" ) == 0;
    if(is_start) entry_point = instructions->index;
    function_labels->push(new Label(function_name,instructions->index));

    instructions->push(new Instruction(NOP));
    instructions->push(new Instruction(PUSH, Parameter(BP, 0)));
    instructions->push(new Instruction(MOV , Parameter(BP, 0), Parameter(SP, 0)));
    
    int stack_size = 0;
    
     // yah ik magic number
    bool has_return_type = strcmp(function_definition->expressions[function_definition->expressions.length-2]->identifier,"ReturnType") == 0;
    if (function_definition->matched_rule->length > 6){
        var_labels->push(new Label(function_definition->expressions[function_definition->expressions.length-((int)has_return_type+3)]->token->content,8));
        stack_size = 8;
        for(int x = 3; x < function_definition->expressions.length-((int)has_return_type+5); x++){
            var_labels->push(new Label(function_definition->expressions[x]->expressions[2]->token->content,8));
            stack_size += 8;
        }
        instructions->push(new Instruction(SUB, Parameter(SP, 0), Parameter(Null, stack_size)));
    }
    parse_statement(expr->expressions[1], &stack_size);
    var_labels->empty();
    if(is_start){
        instructions->push(new Instruction(EXIT));
        return;
    }
    if(instructions->instructions[instructions->index-1]->Type != JMP){
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
    this->function_labels = new LabelList();
    this->var_labels = new LabelList();
    for(int x = 0; x < expr->expressions.length; x++){
        Expression* function_expr = expr->expressions[x];
        parse_function(function_expr);
    }
}

Program::~Program(){
    free(data_section);
    delete function_labels;
    delete var_labels;
    delete instructions;
}