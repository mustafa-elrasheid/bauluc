#include "headers/bytecode.hpp"


Parameter::Parameter(RegisterType type,int offset){
    this->type = type;
    this->offset = offset;
}

Parameter::Parameter(){

}

void Parameter::PrintOffset(const char * str,int offset){
    printf(str);

    if (this->offset > 0) // AKA Positive
        printf("+%d",offset);
    if (this->offset == 0 && type == RegisterType::Null)
        printf("%d",offset);
    if (this->offset < 0)
        printf("%d",offset);// printf does it atuomaticly
    
    
}

void Parameter::log(){
    switch (type){
        case Null:PrintOffset(""  ,this->offset);break;
        case SP  :PrintOffset("SP",this->offset);break;
        case BP  :PrintOffset("BP",this->offset);break;
        case AX  :PrintOffset("AX",this->offset);break;
        case DI  :PrintOffset("DI",this->offset);break;
        case BX  :PrintOffset("BX",this->offset);break;
        case CR  :PrintOffset("CR",this->offset);break;
        default  :PrintOffset("??",this->offset);break;
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
        case MOV       : printf("Mov         ");break;
        case PUSH      : printf("Push        ");break;
        case POP       : printf("Pop         ");break;
        case ADD       : printf("Add         ");break;
        case SUB       : printf("Sub         ");break;
        case DIV       : printf("Div         ");break;
        case MUL       : printf("Mul         ");break;
        case REM       : printf("Rem         ");break;
        case AND       : printf("And         ");break;
        case OR        : printf("Or          ");break;
        case XOR       : printf("Xor         ");break;
        case NOT       : printf("Not         ");break;
        case CMP       : printf("Cmp         ");break;
        case CMPB      : printf("CmpB        ");break;
        case JMP       : printf("Jmp         ");break;
        case NOP       : printf("Nop         ");break;
        case JN        : printf("JN          ");break;
        case DRFRNC    : printf("drfrnc      ");break;
        case SO_CALL   : printf("so_call     ");break;
        case EXIT      : printf("exit        ");break;
        default        : printf("OMG         ");break;
    }     
    printf(" ");
    for(int i = 0 ; i < ParametersNum;i++){
        Parameters[i].log();
        if(i < ParametersNum-1)
            printf(",");
    }
    printf("\n");
}

InstructionList::InstructionList(){
    this->instructions = (Instruction**)malloc(1000*sizeof(Instruction*));
    this->length = 1000;
    this->index = 0;
}

void InstructionList::push(Instruction* instruction){
    this->instructions[index] = instruction;
    this->index++;
}

void InstructionList::log(){
    for(int x = 0; x < this->index; x++){
        printf("%d: ",x);
        this->instructions[x]->log();
    }
}