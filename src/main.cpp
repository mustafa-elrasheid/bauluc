#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "headers/lexer.h"
#include "headers/parser.h"

char* read_file(char* path){
	FILE* file = NULL;
	char* buffer = NULL;
	int file_size = 0;

	file = fopen(path,"r");
	if(file == NULL){
		printf("The File \"%s\" provided doesn't exist\n",path);
		return NULL;
	}

	fseek(file, 0L, SEEK_END);
	file_size=ftell(file);
	buffer = (char*)malloc(file_size+1);
	memset(buffer,0,file_size+1);
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

	printf("file: %s\n",text);

	const char* keywords[35] = {"!","@","#","$","%","^","&","*","(",")","-","+","=","\t"," ","{","}","[","]","\"",":",";","'","\"","<",">",".",",","?","\\","|","/"};
	keywords[34] = NULL;
	
	TokenVector* token_stack = new TokenVector(text,  keywords);
	printf("Tokens:{");
	for(int x= 0; x < token_stack->length; x++){
		if(token_stack->tokens[x]->token[0] == '\n' || token_stack->tokens[x]->token[0] == '\t' || token_stack->tokens[x]->token[0] == ' ') continue;
		printf(" \"%s\", ",token_stack->tokens[x]->token);
	}
	printf("}\n");
	
	Expression* magic_token = new Expression(token_stack,0);
	if(magic_token == NULL){
		printf("Syntax Error\n");
		return -1;
	}
	magic_token->show_tree(0);
	delete magic_token;
	
	for(int x= 0; x < token_stack->length; x++){
		free(token_stack->tokens[x]->token);
		free(token_stack->tokens[x]);
	}
	
	free(token_stack->tokens);
	free(token_stack);
	
	free(text);
	return 0;
}