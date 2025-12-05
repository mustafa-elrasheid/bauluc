#include "headers/bytecode.hpp"

Parameter::Parameter(RegisterType type,int offset){
    this->type = type;
    this->offset = offset;
}

Parameter::Parameter(){}

void Parameter::PrintOffset(const char * str,int offset){
    printf(str);
    printf("%d",offset);
}

void Parameter::log(){
    switch (type){
        case Null: printf("   %d", this->offset); break;
        case SP  : printf("SP %d", this->offset); break;
        case BP  : printf("BP %d", this->offset); break;
        case AX  : printf("AX %d", this->offset); break;
        case DI  : printf("DI %d", this->offset); break;
        case BX  : printf("BX %d", this->offset); break;
        case CR  : printf("CR %d", this->offset); break;
        default  : printf("?? %d", this->offset); break;
    }
}


Instruction::Instruction(InstructionType type){
    ParametersNum = 0;
    Type = type;
}

Instruction::Instruction(InstructionType type,Parameter value){
    ParametersNum = 1;
    Parameters[FIRST] = value;
    Type = type;
}

Instruction::Instruction(InstructionType type,Parameter first_value,Parameter second_value){
    ParametersNum = 2;
    Parameters[FIRST] = first_value;
    Parameters[SECOND] = second_value;
    Type = type;
}

Instruction* Instruction::Clone(){
    void* temp = malloc(sizeof(Instruction));
    return (Instruction*)memcpy(temp,this,sizeof(Instruction));
}

void Instruction::log(){
    switch (Type){     
        case MOV    : printf("Mov         "); break;
        case PUSH   : printf("Push        "); break;
        case POP    : printf("Pop         "); break;
        case ADD    : printf("Add         "); break;
        case SUB    : printf("Sub         "); break;
        case DIV    : printf("Div         "); break;
        case MUL    : printf("Mul         "); break;
        case REM    : printf("Rem         "); break;
        case AND    : printf("And         "); break;
        case OR     : printf("Or          "); break;
        case XOR    : printf("Xor         "); break;
        case NOT    : printf("Not         "); break;
        case CMP    : printf("Cmp         "); break;
        case CMPB   : printf("CmpB        "); break;
        case JMP    : printf("Jmp         "); break;
        case NOP    : printf("Nop         "); break;
        case JN     : printf("JN          "); break;
        case DRFRNC : printf("drfrnc      "); break;
        case SO_CALL: printf("so_call     "); break;
        case EXIT   : printf("exit        "); break;
        default     : printf("OMG         "); break;
    }     
    printf(" ");
    for(int i = 0 ; i < ParametersNum;i++){
        Parameters[i].log();
        if(i < ParametersNum-1)
            printf(",");
    }
    printf("\n");
}

InstructionList::InstructionList(int size){
    this->instructions = (Instruction**)malloc(size*sizeof(Instruction*));
    this->length = size;
    this->index = 0;
}

void InstructionList::push(Instruction* instruction){
    if(this->index >= this->length){
        this->length *= 2;
        this->instructions = (Instruction**) realloc(this->instructions, this->length*sizeof(Instruction*));
    }
    this->instructions[index] = instruction;
    this->index++;
}

void InstructionList::log(){
    for(int x = 0; x < this->index; x++){
        printf("%d: ",x);
        this->instructions[x]->log();
    }
}