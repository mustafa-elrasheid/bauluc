#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
	char** tokens;
	unsigned int length;
}Tokens;

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
	buffer = malloc(file_size);
	fseek(file, 0L, SEEK_SET);
	fread(buffer, 1, file_size, file);

	fclose(file);
	return buffer;
}

bool check_prefix(const char* text,const char* prefix){
	int index = 0;
	while(text[index] != 0 && prefix[index] != 0){
		if (text[index] != prefix[index])return false;
		index ++;
	}
	return true;
}

const char* check_keywords(const char* text, const char** keywords){
	for(int i = 0;keywords[i] != NULL; i++){
		if(check_prefix(text,keywords[i])){
			return keywords[i];
		}
	}
	return NULL;
}

Tokens tokenizer (const char* text,const char** keywords){
	
	int token_count = 0;
	for(int index = 0;text[index] != '\0';){
		if(check_keywords(text+index,keywords)){
			index++;
			continue;
		}
		int word_length = 0;
		for(int y = 0;!check_keywords(text+index+y,keywords);y++)word_length++;
		index += word_length;
		token_count++;
	}
	char** tokens = malloc(token_count*sizeof(char*));
	int token_count2 = 0;
	for(int index = 0;text[index] != '\0';){
		const char* keyword = check_keywords(text+index,keywords);
		if(keyword!= NULL){
			printf("meh: %s\n",keyword);
			tokens[token_count2] = malloc(strlen(keyword));
			strcpy(tokens[token_count2],keyword);
			index++;
			token_count2++;
			continue;
		}
		int word_length = 0;
		for(int y = 0;!check_keywords(text+index+y,keywords);y++)word_length++;
		index += word_length;
	}
}

int main (int argc,char**argv ){
	
	if(argc <= 1){
		printf("No File Included\n");
		return -1;
	}

	char* text = read_file(argv[1]);
	if(text == NULL) return -1;

	printf("file: %s\n",text);

	const char* keywords[35] = {"!","@","#","$","%","^","&","*","(",")","-","_","+","=","\t"," ","{","}","[","]","\"",":",";","'","\"","<",">",".",",","?","\\","|","/"};
	keywords[34] = NULL;
	Tokens tokens = tokenizer(text, keywords);

	free(text);
	return 0;
}