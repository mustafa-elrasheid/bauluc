#include "lexer.h"

Token* create_token(char* token_str,TokenType type){
	Token* token = malloc(sizeof(Token));
	token->token = strdup(token_str);
	token->binding_power_left = 10;
	token->binding_power_right = 11;
	token->type = type;
	return token;
}

char* create_str(const char* str1, int length){
	char* new_str = malloc(length+1);
	strncpy(new_str, str1, length);
	new_str[length] = '\0';
	return new_str;
}

bool check_prefix(const char* text,const char* prefix){
	int index = 0;
	for(;text[index] != 0 && prefix[index] != 0; index ++)
		if (text[index] != prefix[index]) return false;
	return true;
}

const char* check_keywords(const char* text, const char** keywords){
	for(int i = 0; keywords[i] != NULL; i++)
		if(check_prefix(text, keywords[i]))
			return keywords[i];
	return NULL;
}

int count_tokens(const char* text, const char** keywords){
	int token_count = 0;
	for(int index = 0; text[index] != '\0'; token_count++, index++){
		if(check_keywords(text+index,keywords)) continue;
		for(;!check_keywords(text+index,keywords);index++);
		index--;
	}
	return token_count;
}

TokenVector* tokenizer(const char* text,const char** keywords){
	Token** tokens = malloc((count_tokens(text,keywords)+1)*sizeof(Token));
	int token_count = 0;
	for(int index = 0; text[index] != '\0'; index++, token_count++){
		const char* keyword = check_keywords(text+index, keywords);
		if(keyword != NULL){
			tokens[token_count] = create_token(create_str(keyword, strlen(keyword)),OPERATOR);
			continue;
		}
		int word_length = 0;
		for(int y = 0; !check_keywords(text+index+y, keywords); y++) word_length++;
		tokens[token_count] = create_token(create_str(text+index, word_length),ATOM);
		index += word_length-1;
	}
	TokenVector* result = malloc(sizeof(TokenVector));
	result->tokens = tokens;
	result->index = 0;
	result->length = token_count+1;
	result->tokens[token_count] = create_token("EOF",END_OF_FILE);
	return result;
}