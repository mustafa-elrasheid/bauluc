#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "headers/lexer.h"
#include "headers/parser.h"
#include "headers/grammer.h"

char* read_file(char* path){
	FILE* file = fopen(path,"r");
	if(file == NULL){
		printf("The File \"%s\" provided doesn't exist\n",path);
		return NULL;
	}
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
		printf("No File Included\n");
		return -1;
	}

	char* text = read_file(argv[1]);
	if(text == NULL) return -1;
	printf("file:\n%s\n",text);

	const char* keywords[41]  = {">=","<=","||","&&","!=","==","=>","!","@","#","$","%","^","&","*","(",")","-","+","=","\t"," ","{","}","[","]","\"",":",";","'","\"","<",">",".",",","?","\\","|","/","\n",NULL};
	const char* keywords2[]  = {"function","return","if","while","let","else","import","from","as",NULL};

	TokenList* token_stack = new TokenList(text, keywords);
	token_stack->log();
	token_stack->offside("\t","{","}");
	token_stack->log();
	token_stack->remove_whitespace();
	token_stack->log();
	token_stack->flip_to_operator(keywords2);
	token_stack->log();
	
	GrammerRuleList* grammer_rules = new GrammerRuleList(
		new GrammerRule*[83]{
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
			new GrammerRule("IDENTIFIER",          new Token("STR",ATOM)),
			new GrammerRule("NUMBER",              new Token("NUM",ATOM)),
			/* Identifiers*/
			//strings should be there
			new GrammerRule("Parameter",    new const char* [3]{"LET","IDENTIFIER","COMA"},3),
			new GrammerRule("ReturnType",   new const char* [2]{"DOUBLE_ARROW","IDENTIFIER"},2),
			new GrammerRule("FunctionDef",  new const char* [6]{"FUNCTION","IDENTIFIER","ROUND_BRACKET_OPEN","ROUND_BRACKET_CLOSE","ReturnType?","COLON"},6),
			new GrammerRule("FunctionDef",  new const char* [9]{"FUNCTION","IDENTIFIER","ROUND_BRACKET_OPEN","Parameter*","LET","IDENTIFIER","ROUND_BRACKET_CLOSE","ReturnType?","COLON"},9),
			/* Expressions*/
			new GrammerRule("Expression",   new const char* [1]{"IDENTIFIER"},1),
			new GrammerRule("Expression",   new const char* [1]{"NUMBER"},1),
			new GrammerRule("Expression",   new const char* [4]{"Expression","SQUARE_BRACKET_OPEN","Expression",    "SQUARE_BRACKET_CLOSE"},4),
			new GrammerRule("Expression",   new const char* [5]{"Expression","ROUND_BRACKET_OPEN", "ExpressionArg*","Expression?","ROUND_BRACKET_CLOSE"},5),
			new GrammerRule("Expression",   new const char* [3]{"ROUND_BRACKET_OPEN","Expression","ROUND_BRACKET_CLOSE"},3),
			new GrammerRule("Expression",   new const char* [3]{"Expression","MODULO",             "Expression"},3),
			new GrammerRule("Expression",   new const char* [3]{"Expression","EQUAL",              "Expression"},3),
			new GrammerRule("Expression",   new const char* [3]{"Expression","IS_EQUAL",           "Expression"},3),
			new GrammerRule("Expression",   new const char* [3]{"Expression","ISNT_EQUAL",         "Expression"},3),
			new GrammerRule("Expression",   new const char* [3]{"Expression","IS_GREATER",         "Expression"},3),
			new GrammerRule("Expression",   new const char* [3]{"Expression","IS_SMALLER",         "Expression"},3),
			new GrammerRule("Expression",   new const char* [3]{"Expression","IS_GREATER_OR_EQUAL","Expression"},3),
			new GrammerRule("Expression",   new const char* [3]{"Expression","IS_SMALLER_OR_EQUAL","Expression"},3),
			new GrammerRule("Expression",   new const char* [3]{"Expression","DOT",                "Expression"},3),
			new GrammerRule("Expression",   new const char* [3]{"Expression","ARROW",              "Expression"},3),
			new GrammerRule("Expression",   new const char* [3]{"Expression","LOGIC_AND",          "Expression"},3),
			new GrammerRule("Expression",   new const char* [3]{"Expression","LOGIC_OR",           "Expression"},3),
			new GrammerRule("Expression",   new const char* [3]{"Expression","AMPERSAND",          "Expression"},3),
			new GrammerRule("Expression",   new const char* [3]{"Expression","BIT_OR",             "Expression"},3),
			new GrammerRule("Expression",   new const char* [3]{"Expression","BIT_XOR",            "Expression"},3),
			new GrammerRule("Expression",   new const char* [3]{"Expression","CARET",              "Expression"},3),
			new GrammerRule("Expression",   new const char* [3]{"Expression","STAR",               "Expression"},3),
			new GrammerRule("Expression",   new const char* [3]{"Expression","DIV",                "Expression"},3),
			new GrammerRule("Expression",   new const char* [3]{"Expression","PLUS",               "Expression"},3),
			new GrammerRule("Expression",   new const char* [3]{"Expression","SUB",                "Expression"},3),
			new GrammerRule("Expression",   new const char* [2]{"BIT_NOT",  "Expression"},2),
			new GrammerRule("Expression",   new const char* [2]{"LOGIC_NOT","Expression"},2),
			new GrammerRule("Expression",   new const char* [2]{"SUB",      "Expression"},2),
			// pointers
			new GrammerRule("Expression",   new const char* [2]{"STAR",      "Expression"},2),
			new GrammerRule("Expression",   new const char* [2]{"AMPERSAND", "Expression"},2),
			new GrammerRule("ExpressionArg",new const char* [2]{"Expression","COMA"},2),
			/* Statements */
			new GrammerRule("Statement",    new const char* [3]{"RETURN","Expression","LINE_END"},3),
			new GrammerRule("Statement",    new const char* [4]{"IF","Expression","COLON","Statement"},4),
			new GrammerRule("Statement",    new const char* [4]{"WHILE","Expression","COLON","Statement"},4),
			new GrammerRule("Statement",    new const char* [4]{"LET","Expression","Expression","LINE_END"},4),
			new GrammerRule("Statement",    new const char* [2]{"Expression","LINE_END"},2),
			new GrammerRule("Statement",    new const char* [3]{"CURLY_BRACKET_OPEN","Statement*","CURLY_BRACKET_CLOSE"},3),
			/* Structure */
			new GrammerRule("Function",     new const char* [2]{"FunctionDef","Statement"},2),
			new GrammerRule("Functions",    new const char* [1]{"Function*"},1),
		},
		83
	);
	grammer_rules->log();

	ExpressionList* exprs_list = new ExpressionList(token_stack,grammer_rules);
	for(int x = 0; x< 100 && exprs_list->length != 1;x++)
		exprs_list->reduce(grammer_rules);
	exprs_list->log();
	
	delete exprs_list;
	delete grammer_rules;
	delete token_stack;
	free(text);
	return 0;
}