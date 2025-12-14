#include "headers/bytecode.hpp"

Parameter::Parameter(RegisterType type, int offset){
    this->type = type;
    this->offset = offset;
}

Parameter::Parameter(){}

void Parameter::PrintOffset(const char * str, int offset){
    printf(str);
    if(offset != 0) printf("%d", offset);
}

void Parameter::log(){
    switch (type){
        case Null: printf(""  ); break;
        case RSP : printf("RSP"); break;
        case RBP : printf("RBP"); break;
        case RAX : printf("RAX"); break;
        case RDI : printf("RDI"); break;
        case RBX : printf("RBX"); break;
        case RCR : printf("RCR"); break;
        case data: printf("DT"); break;
        default  : printf("??"); break;
    }
    if(type == Null)printf("%d", this->offset);
    else if(offset != 0)printf("%+d", this->offset);
}

Instruction::Instruction(InstructionType type){
    ParametersNum = 0;
    Type = type;
}

Instruction::Instruction(InstructionType type, Parameter value){
    ParametersNum = 1;
    Parameters[FIRST] = value;
    Type = type;
}

Instruction::Instruction(InstructionType type, Parameter first_value, Parameter second_value){
    ParametersNum = 2;
    Parameters[FIRST] = first_value;
    Parameters[SECOND] = second_value;
    Type = type;
}

void Instruction::log(){
    switch (Type){     
        case MOV:     printf("\tmov     "); break;
        case PUSH:    printf("\tpush    "); break;
        case POP:     printf("\tpop     "); break;
        case ADD:     printf("\tadd     "); break;
        case SUB:     printf("\tsub     "); break;
        case DIV:     printf("\tdiv     "); break;
        case MUL:     printf("\tmul     "); break;
        case REM:     printf("\trem     "); break;
        case AND:     printf("\tAnd     "); break;
        case OR:      printf("\tor      "); break;
        case XOR:     printf("\txor     "); break;
        case NOT:     printf("\tnot     "); break;
        case CMP:     printf("\tcmp     "); break;
        case CMPB:    printf("\tcmpB    "); break;
        case JMP:     printf("\tjmp     "); break;
        case NOP:     printf("nop       "); break;
        case JN:      printf("\tjn      "); break;
        case DRFRNC:  printf("\tdrfrnc  "); break;
        case SO_CALL: printf("\tso_call "); break;
        case EXIT:    printf("\texit    "); break;
        case LEAVE:   printf("\tleave   "); break;
        case RET:     printf("\tret     "); break;
        case CALL:    printf("\tcall    "); break;
        default:      printf("\tOMG     "); break;
    }     
    printf(" ");
    for(int i = 0 ; i < ParametersNum; i++){
        Parameters[i].log();
        if(i < ParametersNum-1)
            printf(", ");
    }
    printf("\n");
}

InstructionList::InstructionList(int size){
    this->instructions = (Instruction**) malloc(size*sizeof(Instruction*));
    this->length = size;
    this->index = -1;
}

void InstructionList::push(Instruction* instruction){
    if(this->index >= this->length){
        this->length *= 2;
        this->instructions = (Instruction**) realloc(this->instructions, this->length*sizeof(Instruction*));
    }
    this->index++;
    this->instructions[index] = instruction;
}

void InstructionList::log(){
    for(int x = 0; x < this->index+1; x++){
        printf("%d: ", x);
        this->instructions[x]->log();
    }
}

Instruction* InstructionList::peak(){
    return this->instructions[index];
}

InstructionList::~InstructionList(){
    for(int x = 0; x < this->index+1; x++)
        delete this->instructions[x];
    free(instructions);
}