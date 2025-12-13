#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "frontend/lexer.hpp"
#include "frontend/parser.hpp"
#include "frontend/grammer.hpp"
#include "headers/vm.hpp"
#include "headers/program.hpp"

using namespace Lexer;

char* read_file(char* path){
	FILE* file = fopen(path,"r");
	if(file == NULL)throw "file not found";
	fseek(file, 0L, SEEK_END);
	int file_size = ftell(file);
	char* buffer = (char*)calloc(1, file_size+1);
	fseek(file, 0L, SEEK_SET);
	fread(buffer, 1, file_size, file);
	fclose(file);
	return buffer;
}

int main (int argc,char**argv ){
	
	if(argc <= 1){
		printf("No File Included. for more info use \"-help\".\n");
		return -1;
	}

	bool show_log = false;
	bool show_exit_code = false;
	for(int i = 0; i < argc; i++){
		if(strcmp("-info",argv[i]) == 0){
			printf("Baulu Compiler\na compiler for the baulu programming language\n");
			return 0;
		}
		if(strcmp("-help",argv[i]) == 0){
			printf("Usage: baulo <source-file> [options]\n");
			printf("Options:\n");
			printf("\t-info     :\tDisplay information about the compiler\n");
			printf("\t-help     :\tDisplay this help message\n");
			printf("\t-log      :\tEnable detailed logging during compilation and execution\n");
			printf("\t-exit-code:\tDisplay the exit code for the program");
			printf("\t-version  :\tDisplay the compiler version\n");
			return 0;
		}
		if(strcmp("-log",argv[i]) == 0){
			show_log = true;
		}
		if(strcmp("-version",argv[i]) == 0){
			printf("Baulu Compiler version 0.1.0\n");
			return 0;
		}
		if(strcmp("-exit-code",argv[i]) == 0){
			show_exit_code = true;
		}
	}

	char* text;
	try{
		text = read_file(argv[1]);
		const char* suffix = "\nfunction _start():{main();}";
		text = (char*)realloc(text, strlen(text)+strlen(suffix)+1);
		strcat(text, suffix);
		if(show_log) printf("file:\n%s\n",text);
	}catch (const char* message){
		printf("File error: %s.\n",message);
		return -1;
	}

	const char* splitters[]  = {"\"[^\"]+\"","#[^\n]+\n",">=","<=","\\|\\|","&&","!=","==","=>","!","@","#","\\$","%","\\^","&","\\*","\\(","\\)","-","\\+","=","\t","    "," ","{","}","\\[","\\]",":",";","'","<",">","\\.",",","\\?","\\\\","\\|","\\/","\n",NULL};
	const char* keywords[]  = {"function","return","if","while","let","else","import","from","as",NULL};
	const char* denters[] = {"\t","    ",NULL};
	
	TokenList* token_stack;
	try{
		token_stack = new TokenList(text, splitters);
		if(show_log)token_stack->log();
		token_stack->offside(denters,"{","}");
		if(show_log)token_stack->log();
		token_stack->remove_whitespace();
		if(show_log)token_stack->log();
		token_stack->flip_to_operator(keywords);
		if(show_log)token_stack->log();
	} catch(const char* message){
		printf("Lexing Error: %s.\n",message);
		return -1;
	}
	free(text);

	GrammerRuleList* token_rules;
	GrammerRuleList* grammer_rules;
	try{
		token_rules = new GrammerRuleList(
			new GrammerRule*[41]{
				/* keywords */
				// functions
				new GrammerRule("FUNCTION",            new Token("function",OPERATOR)),
				new GrammerRule("RETURN",              new Token("return",OPERATOR)),
				// control flow
				new GrammerRule("IF",                  new Token("if",OPERATOR)),
				new GrammerRule("WHILE",               new Token("while",OPERATOR)),
				new GrammerRule("LET",                 new Token("let",OPERATOR)),
				new GrammerRule("ELSE",                new Token("else",OPERATOR)),
				// imports
				new GrammerRule("IMPORT",              new Token("import",OPERATOR)),
				new GrammerRule("FROM",                new Token("from",OPERATOR)),
				new GrammerRule("AS",                  new Token("as",OPERATOR)),
				/* Symbols */
				// boolean Logic
				new GrammerRule("IS_EQUAL",            new Token("==",OPERATOR)),
				new GrammerRule("ISNT_EQUAL",          new Token("!=",OPERATOR)),
				new GrammerRule("IS_GREATER",          new Token(">",OPERATOR)),
				new GrammerRule("IS_SMALLER",          new Token("<",OPERATOR)),
				new GrammerRule("IS_GREATER_OR_EQUAL", new Token(">=",OPERATOR)),
				new GrammerRule("IS_SMALLER_OR_EQUAL", new Token("<=",OPERATOR)),
				new GrammerRule("LOGIC_AND",           new Token("&&",OPERATOR)),
				new GrammerRule("LOGIC_OR",            new Token("||",OPERATOR)),
				new GrammerRule("LOGIC_NOT",           new Token("!",OPERATOR)),
				// Bitwise Operations
				new GrammerRule("BIT_OR",              new Token("|",OPERATOR)),
				new GrammerRule("BIT_NOT",             new Token("~",OPERATOR)),
				new GrammerRule("AMPERSAND",           new Token("&",OPERATOR)),
				// Definitions
				new GrammerRule("DOUBLE_ARROW",        new Token("=>",OPERATOR)),
				new GrammerRule("COMA",                new Token(",",OPERATOR)),
				new GrammerRule("COLON",               new Token(":",OPERATOR)),
				new GrammerRule("LINE_END",            new Token(";",OPERATOR)),
				// Arithmatics
				new GrammerRule("EQUAL",               new Token("=",OPERATOR)),
				new GrammerRule("CARET",               new Token("^",OPERATOR)),
				new GrammerRule("STAR",                new Token("*",OPERATOR)),
				new GrammerRule("DIV",                 new Token("/",OPERATOR)),
				new GrammerRule("MODULO",              new Token("%",OPERATOR)),
				new GrammerRule("PLUS",                new Token("+",OPERATOR)),
				new GrammerRule("SUB",                 new Token("-",OPERATOR)),
				// brackets
				new GrammerRule("ROUND_BRACKET_OPEN",  new Token("(",OPERATOR)),
				new GrammerRule("ROUND_BRACKET_CLOSE", new Token(")",OPERATOR)),
				new GrammerRule("SQUARE_BRACKET_OPEN", new Token("[",OPERATOR)),
				new GrammerRule("SQUARE_BRACKET_CLOSE",new Token("]",OPERATOR)),
				new GrammerRule("CURLY_BRACKET_OPEN",  new Token("{",OPERATOR)),
				new GrammerRule("CURLY_BRACKET_CLOSE", new Token("}",OPERATOR)),
				new GrammerRule("IDENTIFIER",          new Token("IDF",ATOM)),
				new GrammerRule("NUMBER",              new Token("NUM",ATOM)),
				new GrammerRule("STRING",              new Token("STR",OPERATOR)),
			},
			41
		);
		grammer_rules = new GrammerRuleList(
			new GrammerRule*[44]{
				/* Identifiers*/
				//strings should be there
				new GrammerRule("Parameter",    new const char* [4]{"LET","IDENTIFIER","IDENTIFIER","COMA"},4),
				new GrammerRule("ReturnType",   new const char* [2]{"DOUBLE_ARROW","IDENTIFIER"},2),
				new GrammerRule("FunctionDef",  new const char* [6]{"FUNCTION","IDENTIFIER","ROUND_BRACKET_OPEN","ROUND_BRACKET_CLOSE","ReturnType?","COLON"},6),
				new GrammerRule("FunctionDef",  new const char* [10]{"FUNCTION","IDENTIFIER","ROUND_BRACKET_OPEN","Parameter*","LET","IDENTIFIER","IDENTIFIER","ROUND_BRACKET_CLOSE","ReturnType?","COLON"},10),
				/* Expressions*/
				new GrammerRule("Expression",   new const char* [1]{"IDENTIFIER"},1),
				new GrammerRule("Expression",   new const char* [1]{"NUMBER"},1),
				new GrammerRule("Expression",   new const char* [1]{"STRING"},1),
				new GrammerRule("Expression",   new const char* [4]{"Expression","SQUARE_BRACKET_OPEN","Expression",    "SQUARE_BRACKET_CLOSE"},4),
				new GrammerRule("Expression",   new const char* [5]{"Expression","ROUND_BRACKET_OPEN", "ExpressionArg*","Expression?","ROUND_BRACKET_CLOSE"},5),
				new GrammerRule("Expression",   new const char* [3]{"ROUND_BRACKET_OPEN","Expression","ROUND_BRACKET_CLOSE"},3),
				new GrammerRule("Expression",   new const char* [3]{"Expression", "MODULO",              "Expression"}, 3),
				new GrammerRule("Expression",   new const char* [3]{"Expression", "IS_EQUAL",            "Expression"}, 3),
				new GrammerRule("Expression",   new const char* [3]{"Expression", "ISNT_EQUAL",          "Expression"}, 3),
				new GrammerRule("Expression",   new const char* [3]{"Expression", "IS_GREATER",          "Expression"}, 3),
				new GrammerRule("Expression",   new const char* [3]{"Expression", "IS_SMALLER",          "Expression"}, 3),
				new GrammerRule("Expression",   new const char* [3]{"Expression", "IS_GREATER_OR_EQUAL", "Expression"}, 3),
				new GrammerRule("Expression",   new const char* [3]{"Expression", "IS_SMALLER_OR_EQUAL", "Expression"}, 3),
				new GrammerRule("Expression",   new const char* [3]{"Expression", "DOT",                 "Expression"}, 3),
				new GrammerRule("Expression",   new const char* [3]{"Expression", "ARROW",               "Expression"}, 3),
				new GrammerRule("Expression",   new const char* [3]{"Expression", "LOGIC_AND",           "Expression"}, 3),
				new GrammerRule("Expression",   new const char* [3]{"Expression", "LOGIC_OR",            "Expression"}, 3),
				new GrammerRule("Expression",   new const char* [3]{"Expression", "AMPERSAND",           "Expression"}, 3),
				new GrammerRule("Expression",   new const char* [3]{"Expression", "BIT_OR",              "Expression"}, 3),
				new GrammerRule("Expression",   new const char* [3]{"Expression", "BIT_XOR",             "Expression"}, 3),
				new GrammerRule("Expression",   new const char* [3]{"Expression", "CARET",               "Expression"}, 3),
				new GrammerRule("Expression",   new const char* [3]{"Expression", "STAR",                "Expression"}, 3),
				new GrammerRule("Expression",   new const char* [3]{"Expression", "DIV",                 "Expression"}, 3),
				new GrammerRule("Expression",   new const char* [3]{"Expression", "PLUS",                "Expression"}, 3),
				new GrammerRule("Expression",   new const char* [3]{"Expression", "SUB",                 "Expression"}, 3),
				new GrammerRule("Expression",   new const char* [3]{"Expression", "EQUAL",               "Expression"}, 3),
				// unary operations
				new GrammerRule("Expression",   new const char* [2]{"BIT_NOT",    "Expression"},2),
				new GrammerRule("Expression",   new const char* [2]{"LOGIC_NOT",  "Expression"},2),
				new GrammerRule("Expression",   new const char* [2]{"SUB",        "Expression"},2),
				// pointers
				new GrammerRule("Expression",   new const char* [2]{"STAR",       "Expression"},2),
				new GrammerRule("Expression",   new const char* [2]{"AMPERSAND",  "Expression"},2),
				// function call arguments
				new GrammerRule("ExpressionArg",new const char* [2]{"Expression", "COMA"},2),
				/* Statements */
				new GrammerRule("Statement",    new const char* [4]{"IF",    "Expression", "COLON", "Statement"},4),
				new GrammerRule("Statement",    new const char* [4]{"WHILE", "Expression", "COLON", "Statement"},4),
				new GrammerRule("Statement",    new const char* [4]{"LET",   "Expression", "Expression", "LINE_END"},4),
				new GrammerRule("Statement",    new const char* [3]{"RETURN","Expression",               "LINE_END"},3),
				new GrammerRule("Statement",    new const char* [2]{         "Expression",               "LINE_END"},2),
				new GrammerRule("Statement",    new const char* [3]{"CURLY_BRACKET_OPEN","Statement*","CURLY_BRACKET_CLOSE"},3),
				/* Structure */
				new GrammerRule("Function",     new const char* [2]{"FunctionDef", "Statement"},2),
				new GrammerRule("Functions",    new const char* [1]{"Function*"},1),
			},
			44
		);
		if(show_log){
			token_rules->log();
			grammer_rules->log();
		}
	} catch(const char* message){
		printf("Grammer Error: %s.\n",message);
		return -1;
	}

	ExpressionList* exprs_list;
	try{
		exprs_list = new ExpressionList(token_stack, token_rules);
		exprs_list->reduce(grammer_rules);
		if(show_log)exprs_list->log();
		if(exprs_list->length > 1) throw "Parsing not finished";
	} catch (const char* message){
		printf("Parsing Error: %s.\n",message);
		return -1;
	}

	Program* program;
	try
	{
		program = new Program(exprs_list->expressions[0]);
		if(show_log)program->instructions->log();
	} catch(const char* message){
		printf("Program Error: %s.\n",message);
		return -1;
	}
	delete token_stack;
	delete exprs_list;
	delete token_rules;
	delete grammer_rules;

	VirtualMachine* vm;
	try{
		vm = new VirtualMachine(program->entry_point, program->data_section, 1000, 20);
		vm->Run(program->instructions, show_log);
		printf("Program terminated with exit code: %d\n",vm->Registers[RegisterType::AX]);
	} catch(const char* message){
		printf("Runtime Error: %s\n",message);
		return -1;
	}
	delete program;
	delete vm;

	return 0;
}