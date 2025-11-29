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
	MOV,  //  0 , 2 
	/* Binary Operators  */
	ADD  , // 0 , 2
	SUB  , // 0 , 2
	DIV  , // 0 , 2
	MUL  , // 0 , 2
	REM  , // 0 , 2
	AND  , // 0 , 2
	OR   , // 0 , 2
	XOR  , // 0 , 2
	NOT  , // 0 , 2
	CMP  , // 0 , 2
	CMPB , // 0 , 2
	/* control flow */
	JMP, // 1 ( pop only accept regesters)
	NOP, // 0
	JN,  // 1
	/* stack related */
	PUSH,      // 1
	POP,       // 1
	/* lea */
	DRFRNC, // 0
	/* other */
	SO_CALL, // 2 first: function name and library pointer, second: agrument number
	EXIT,    // special exit code
};

enum RegisterType{
	Null,
	data,
	SP, // stack pointer register
	BP, // base pointer register
	AX, // return value register
	DI, // general use register
	BX, // general use register
	CR, // comparetion register
	IP
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
};