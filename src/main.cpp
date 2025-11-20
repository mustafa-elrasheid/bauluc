#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "headers/lexer.h"
#include "headers/parser.h"

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

	TokenVector* token_stack = new TokenVector(text, keywords);
	token_stack->log();
	
	Expression* magic_token = new Expression(token_stack,0);
	magic_token->log(0);

	delete magic_token;
	delete token_stack;
	
	free(text);
	return 0;
}