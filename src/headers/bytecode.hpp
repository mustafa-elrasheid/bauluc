#pragma once

#include <stdio.h>
#include <stdint.h>
#include <cstdlib>
#include <memory.h>

#define FIRST 0
#define SECOND 1

enum InstructionType{
	// 0 for no  parameter
	// 1 for one parameter
	// 2 for two parameters
	/* Binary Operators  */
	MOV,  //  would move the top element in the stack to the address provided below or move from register to register
	ADD,  // adds two elements at the top of the stack or adds one register to the other
	SUB,  // subtracts two elements at the top of the stack or subtracts one register to the other
	DIV,  // divides two elements at the top of the stack or divides one register to the other
	MUL,  // multiplies two elements at the top of the stack or multiplies one register to the other
	REM,  // modul two elements at the top of the stack or modul one register to the other
	AND,  // and two elements at the top of the stack or and one register to the other
	OR,   // bitwise or two elements at the top of the stack or bitwise or one register to the other
	XOR,  // xor two elements at the top of the stack or xor one register to the other
	NOT,  // bitwise flip the element at the top of the stack or flips one register
	CMP,  // compares two elements at the top of the stack or compares one register to the other
	CMPB, // compares who's bigger two elements at the top of the stack or compares who's bigger one register to the other
	/* control flow */
	JMP, // 1 ( pop only accept regesters)
	NOP, // does nothing
	JN,  // jumps to address of CR is 0
	/* stack related */
	PUSH, // pushes and element to the stack
	POP,  // pops an element from the stock to a register
	DRFRNC, // dereferences the value at the top of the stack
	/* other */
	SO_CALL, // 2 first: function name and library pointer, second: agrument number
	EXIT,    // special exit code
};

enum RegisterType{
	Null, // provides no register, used as offset
	data, // data section pointer
	SP,   // stack pointer register
	BP,   // base pointer register
	AX,   // return value register
	DI,   // general use register
	BX,   // general use register
	CR,   // comparetion register
	IP    // instruction pointer
};

struct Parameter{
	RegisterType type;
	int64_t offset;
	Parameter(RegisterType type,int offset);
	Parameter();
	void PrintOffset(const char * str,int offset);
	void log();
};

struct Instruction{
	InstructionType Type;
	int ParametersNum;
	Parameter Parameters[2];
	Instruction(InstructionType type);
	Instruction(InstructionType type,Parameter value);
	Instruction(InstructionType type,Parameter first_value,Parameter second_value);
	Instruction* Clone();
	void log();
};

struct InstructionList{
	Instruction** instructions;
	int length;
	int index;
	InstructionList();
	void push(Instruction* instruction);
	void log();
	~InstructionList();
};