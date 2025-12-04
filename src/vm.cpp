#include "headers/vm.hpp"

int64_t call_so_lib_function(const char* library, const char* function,int* args){
	return (
		(int64_t(*)(int64_t,int64_t,int64_t,int64_t,int64_t,int64_t))
		dlsym(
			dlopen(library, RTLD_LAZY),
			function
		))
	(args[0],args[1],args[2],args[3],args[4],args[5]);
}

inline int64_t VirtualMachine::parameter_value(Parameter* param){
	return param->offset + Registers[param->type];
}

inline int64_t VirtualMachine::binary_operator(InstructionType type,int64_t value1,int64_t value2){
    switch(type){
        case ADD : return  value1 +  value2;
        case SUB : return  value1 -  value2;
        case DIV : return  value1 /  value2;
        case MUL : return  value1 *  value2;
        case REM : return  value1 %  value2;
        case AND : return  value1 &  value2;
        case OR  : return  value1 |  value2;
        case XOR : return  value1 ^  value2;
        case NOT : return !value1          ;
        case CMP : return  value1 == value2;
        case CMPB: return  value1 >  value2;
    }
    return 0;
}

VirtualMachine::VirtualMachine(int64_t entrypoint,char* data_Section,int64_t stack_size,int64_t registers_num){
	Stack = (int64_t*)calloc(stack_size*sizeof(int64_t),1);
	_stack_size = stack_size;
	Registers = (int64_t*)calloc(registers_num*sizeof(int64_t),1);
	Registers[RegisterType::SP]   = (int64_t)&Stack[stack_size];
	Registers[RegisterType::SP]   = (int64_t)&Stack[stack_size];
	Registers[RegisterType::data] = (int64_t)data_Section;
	Registers[RegisterType::IP]   = entrypoint;
	Registers[RegisterType::Null] = 0;
}

void VirtualMachine::Run(InstructionList* instructions,bool log){
	for(;true;Registers[RegisterType::IP]++){
		Instruction* instruction = instructions->instructions[Registers[RegisterType::IP]];
		int64_t* args;
		int args_count;
		if(log){
			printf("[");
			for(int x = 0;x < 40;x++){
				printf(" ");
				if(Registers[RegisterType::SP] == (int64_t)&Stack[_stack_size - x])
					printf("SP:");
				if(Registers[RegisterType::BP] == (int64_t)&Stack[_stack_size - x])
					printf("BP:");
				printf("%#x ",Stack[_stack_size - x]);
			}
			printf("]\n");
			printf("AX:%#x ",Registers[RegisterType::AX]);
			printf("IP:%#x ",Registers[RegisterType::IP]);
			printf("SP:%#x ",Registers[RegisterType::SP]);
			instruction->log();
		}
		switch (instruction->Type){
			case DRFRNC:
				address_as_int stack_top;
				stack_top.value = *(int64_t*)Registers[RegisterType::SP];
				*(int64_t*)Registers[RegisterType::SP] = *(stack_top.address);
				break;
			case JMP:
				Registers[RegisterType::IP] = parameter_value(&(instruction->Parameters[FIRST]));
				break;
			case NOP:
				break;
			case JN:
				if(Registers[RegisterType::CR])break;
				Registers[RegisterType::IP] = parameter_value(&(instruction->Parameters[FIRST]));
				break;
			case PUSH:
				Registers[RegisterType::SP]-=8;
				*(int64_t*)Registers[RegisterType::SP] = parameter_value(&(instruction->Parameters[FIRST]));
				break;
			case POP:
				Registers[instruction->Parameters[FIRST].type] = *(int64_t*)Registers[RegisterType::SP];
				Registers[RegisterType::SP]+=8;
				break;
			case MOV:
				if(instruction->ParametersNum == 2){
					Registers[instruction->Parameters[FIRST].type] = parameter_value(&(instruction->Parameters[SECOND]));
					break;
				}
				address_as_int temp;
				temp.value = *(int64_t*)(Registers[RegisterType::SP]+8);
				*temp.address = *(int64_t*)Registers[RegisterType::SP];
				Registers[RegisterType::SP]+=8;
				break;
			case EXIT:
				return;
			case SO_CALL:
				args_count = parameter_value(&(instruction->Parameters[SECOND]));
				// parameter_value(&(instruction->Parameters[SECOND]));
				//if(args_count > 6)break;
				//int64_t args[6];
				//for(int i = 0; i < args_count; i++,Registers[RegisterType::SP]+=8)
				//	args[args_count-i-1] = *(int64_t*)Registers[RegisterType::SP];
				//Registers[RegisterType::AX] = (*Dependencies)[parameter_value(&(instruction->Parameters[FIRST]))]->Run(args,args_count);
				break;
			/* binary operators */
			default:
				if(instruction->ParametersNum == 2){
					Registers[instruction->Parameters[FIRST].type] = binary_operator(
						instruction->Type,
						parameter_value(&(instruction->Parameters[FIRST])),
						parameter_value(&(instruction->Parameters[SECOND]))
					);
					break;
				}
				*(int64_t*)(Registers[RegisterType::SP]+8) = binary_operator(
					instruction->Type,
					*(int64_t*)(Registers[RegisterType::SP]+8),
					*(int64_t*)Registers[RegisterType::SP]
				);
				Registers[RegisterType::SP]+=8;
				break;
		}
	}
}

VirtualMachine::~VirtualMachine(){
	delete[] Stack;
	delete[] Registers;
}