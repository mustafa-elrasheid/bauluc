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

	const char* keywords[35]  = {"!","@","#","$","%","^","&","*","(",")","-","+","=","\t"," ","{","}","[","]","\"",":",";","'","\"","<",">",".",",","?","\\","|","/",NULL};

	TokenList* token_stack = new TokenList(text, keywords);
	token_stack->log();

	GrammerRuleList* grammer_rules = new GrammerRuleList(
		new GrammerRule*[3]{
			new GrammerRule("Expression",new Token(strdup("ANY"),ATOM)),
			new GrammerRule("PLUS",new Token(strdup("+"),OPERATOR)),
			new GrammerRule("Expression",new const char* [3]{"Expression","PLUS","Expression"},3)
		},
		3
	);

	ExpressionList* exprs_list = new ExpressionList(token_stack,grammer_rules);
	exprs_list->log();
	exprs_list->reduce(grammer_rules);
	exprs_list->log();
	exprs_list->reduce(grammer_rules);
	exprs_list->log();
	
	delete grammer_rules;
	delete token_stack;
	free(text);
	return 0;
}