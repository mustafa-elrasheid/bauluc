#include "lexer.hpp"

char* Lexer::create_str(const char* str1, int length){
	char* new_str = (char*)malloc(length+1);
	strncpy(new_str, str1, length);
	new_str[length] = '\0';
	return new_str;
}

int Lexer::check_prefix(const char* text, const char* prefix){
	int text_index = 0;
	for(int prefix_index = 0;prefix[prefix_index] != 0; prefix_index++,text_index++){
		if(prefix[prefix_index] == '['){
			bool flip = false;
			prefix_index++;
			if(prefix[prefix_index] == '^'){
				flip = true;
				prefix_index++;
			}
			bool plus_dec = false;
			int i = 0;
			for(;prefix[prefix_index+i] != ']';i++);
			if(prefix[prefix_index+i+1] == '+') plus_dec = true;
			for(int old_prefix_index = prefix_index; prefix[prefix_index] != ']' && text[text_index] != NULL;prefix_index++){
				if ((text[text_index] == prefix[prefix_index] && !flip) || (text[text_index] != prefix[prefix_index] && flip)){
					text_index++;
					if(plus_dec) prefix_index=old_prefix_index-1;
					else prefix_index+=i;
				}
			}
			if(plus_dec)prefix_index++;
			text_index-=1;
			continue;
		}
		if(prefix[prefix_index] == '\\') prefix_index++;
		if(text[text_index] != prefix[prefix_index]) return 0;
	}
	return text_index;
}

const char* Lexer::check_keywords(const char* text, const char** keywords){
	for(int i = 0; keywords[i] != NULL; i++){
		int prefix_length = check_prefix(text, keywords[i]);
		if(prefix_length != 0)
			return create_str(text,prefix_length);
	}
	return NULL;
}

int Lexer::count_tokens(const char* text, const char** keywords){
	int token_count = 0;
	for(int index = 0; text[index] != '\0'; token_count++){
		if(check_keywords(text+index,keywords)){index++;continue;}
		for(;!check_keywords(text+index,keywords) && text[index]!= '\0';index++);
	}
	return token_count;
}

Lexer::Token::Token(const char* token_str,TokenType type){
	this->content = strdup(token_str);
	this->type = type;
	if(type == END_OF_FILE){
	}
}

Lexer::Token::~Token(){
	free(this->content);
}

Lexer::TokenList::TokenList(const char* text,const char** keywords){
	Token** tokens = (Token**)malloc((count_tokens(text, keywords)+1) * sizeof(Token*));
	int token_count = 0;
	for(int index = 0; text[index] != '\0'; token_count++){
		const char* keyword = check_keywords(text+index, keywords);
		if(keyword != NULL){
			tokens[token_count] = new Token(keyword, OPERATOR);
			index+=strlen(keyword);
			continue;
		}
		int word_length = 0;
		for(; !check_keywords(text+index+word_length, keywords) && text[index+word_length]!= '\0'; word_length++);
		char* atom_str = create_str(text+index, word_length);
		tokens[token_count] = new Token(atom_str, ATOM);
		free(atom_str);
		index += word_length;
	}
	this->tokens = tokens;
	this->index = 0;
	this->length = token_count+1;
	this->tokens[token_count] = new Token("EOF", END_OF_FILE);
}

Lexer::Token*& Lexer::TokenList::operator[](int i) {
	return tokens[i];
}

Lexer::Token* Lexer::TokenList::operator[](int i) const {
	return tokens[i];
}

const Lexer::Token* Lexer::TokenList::next(){
    Token* value = this->tokens[this->index];
    this->index++;
    return value;
}

const Lexer::Token* Lexer::TokenList::peek(){
    return this->tokens[this->index];
}

Lexer::TokenList::~TokenList(){
	for(int x= 0; x < this->length; x++)
		delete this->tokens[x];
	free(this->tokens);
}

void Lexer::TokenList::log(){
	printf("[");
	for(int x= 0; x < this->length; x++){
		const char* token_content = this->tokens[x]->content;
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
		if(x == length-1) printf("\"%s\"", token_content);
		else printf("\"%s\", ", token_content);
	}
	printf("]\n");
}

void Lexer::TokenList::flip_to_operator(const char** keywords){
	for(int x = 0; x < this->length; x++)
		for(int i = 0; keywords[i] != 0; i++)
			if(strcmp(this->tokens[x]->content,keywords[i]) == 0)this->tokens[x]->type = OPERATOR;
}

void Lexer::TokenList::remove_whitespace(){
	for(int x = 0; x < this->length; x++){
		Token* token = this->tokens[x];
		const char* keywords[] = {" ", "\t", "\n","#[^\n]+\n", NULL};
		if(check_keywords(token->content,keywords) != NULL){
			memcpy(
				&this->tokens[x],
				&this->tokens[x+1],
				sizeof(Token*)*(this->length-x-1)
			);
			this->length-=1;
			x-=1;
			delete token;
		}
	}
}

void Lexer::TokenList::offside(const char** keywords, const char* indent, const char* dedent){
	const char* newline[] = {"\n", NULL};
	int prev_tab_count = 0;
	for(int x = 0; x < this->length; x++){
		int tab_count = 0;
		if(!check_keywords(this->tokens[x]->content, newline)) continue;
		for(int i = x+1; i < this->length; i++, tab_count++)
			if(check_keywords(this->tokens[i]->content,keywords)==NULL) break;
		if(x+tab_count+1 < this->length && check_keywords(this->tokens[x+tab_count+1]->content, newline)) continue;
		for(int i = prev_tab_count; i < tab_count; i++)
			this->insert(new Token(indent,OPERATOR),x);
		for(int i = prev_tab_count; i > tab_count; i--)
			this->insert(new Token(dedent,OPERATOR),x);
		prev_tab_count = tab_count;
	}
	for(int i = 0; i < prev_tab_count; i++)
		this->insert(new Token(dedent,OPERATOR), this->length-1);
}

void Lexer::TokenList::insert(Token* token, int index){
	this->tokens = (Token**) realloc(this->tokens, sizeof(Token*)*(this->length+1));
	memcpy(
		&this->tokens[index+1],
		&this->tokens[index],
		sizeof(Token*)*(this->length-index)
	);
	this->tokens[index] = token;
	this->length+=1;
}