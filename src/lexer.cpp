#include "headers/lexer.h"

char* create_str(const char* str1, int length){
	char* new_str = (char*)malloc(length+1);
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

Token::Token(const char* token_str,TokenType type){
	this->token = strdup(token_str);
	this->binding_power_left = 10;
	this->binding_power_right = 11;
	this->type = type;
	if(type == END_OF_FILE){
		this->binding_power_left = -10;
		this->binding_power_right = -10;
	}
}

Token::~Token(){
	free(this->token);
}

TokenList::TokenList(const char* text,const char** keywords){
	Token** tokens = (Token**)malloc((count_tokens(text,keywords)+1)*sizeof(Token*));
	int token_count = 0;
	for(int index = 0; text[index] != '\0'; index++, token_count++){
		const char* keyword = check_keywords(text+index, keywords);
		if(keyword != NULL){
			tokens[token_count] = new Token(keyword,OPERATOR);
			index+=strlen(keyword)-1;
			continue;
		}
		int word_length = 0;
		for(int y = 0; !check_keywords(text+index+y, keywords); y++) word_length++;
		char* atom_str = create_str(text+index, word_length);
		tokens[token_count] = new Token(atom_str,ATOM);
		free(atom_str);
		index += word_length-1;
	}
	this->tokens = tokens;
	this->index = 0;
	this->length = token_count+1;
	this->tokens[token_count] = new Token("EOF",END_OF_FILE);
}

const Token* TokenList::next(){
    Token* value = this->tokens[this->index];
    this->index++;
    return value;
}

const Token* TokenList::peek(){
    return this->tokens[this->index];
}

TokenList::~TokenList(){
	for(int x= 0; x < this->length; x++)
		delete this->tokens[x];
	free(this->tokens);
}

void TokenList::log(){
	printf("[");
	for(int x= 0; x < this->length; x++){
		const char * token = this->tokens[x]->token;
		switch (this->tokens[x]->type){
			case OPERATOR:
				printf("OP:");
				break;
			case ATOM:
				printf("ATOM:");
				break;
			default:
				break;
		}
		if(x == length-1) printf("\"%s\"",token);
		else printf("\"%s\", ",token);
	}
	printf("]\n");
}

void TokenList::flip_to_operator(const char** keywords){
	for(int x = 0; x < this->length; x++){
		Token* token = this->tokens[x];
		for(int x = 0; keywords[x] != 0; x++)
		if(strcmp(token->token,keywords[x]) == 0)token->type = OPERATOR;
	}
}

void TokenList::remove_whitespace(){
	for(int x = 0; x < this->length; x++){
		Token* token = this->tokens[x];
		const char* keywords[4] = {" ","\t","\n",NULL};
		if(check_keywords(token->token,keywords)!=NULL){
			memcpy(
				&this->tokens[x],
				&this->tokens[x+1],
				sizeof(Token*)*(this->length-x-1)
			);
			this->length-=1;
			x-=1;
		}
	}
}

void TokenList::offside(const char* whitespace, const char* indent, const char* dedent){
	const char* keywords[2] = {whitespace,NULL};
	int prev_tab_count = 0;
	for(int x = 0; x < this->length; x++){
		const char* newline[2] = {"\n",NULL};
		if(!check_keywords(this->tokens[x]->token,newline))continue;
		int tab_count = 0;
		for(int i = x+1; i < this->length; x++,i++){
			Token* token = this->tokens[i];
			if(check_keywords(token->token,keywords)==NULL)break;
			tab_count++;
		}
		if(x+tab_count+1 < this->length)if(check_keywords(this->tokens[x+tab_count+1]->token,newline))continue;
		for(int i = prev_tab_count; i < tab_count; i++)this->insert(new Token(indent,OPERATOR),x);
		for(int i = prev_tab_count; i > tab_count; i--)this->insert(new Token(dedent,OPERATOR),x);
		prev_tab_count = tab_count;
	}
	for(int i = 0; i < prev_tab_count; i++)this->insert(new Token(dedent,OPERATOR),this->length-1);
}

void TokenList::insert(Token* token, int index){
	this->tokens = (Token**)realloc(this->tokens,sizeof(Token*)*(this->length+1));
	memcpy(
		&this->tokens[index+1],
		&this->tokens[index],
		sizeof(Token*)*(this->length-index)
	);
	this->tokens[index] = token;
	this->length+=1;
}