#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

typedef enum{
    ATOM,
    OPERATOR,
    END_OF_FILE
}TokenType;

typedef struct{
    char* token;
    TokenType type;
    int binding_power_left;
    int binding_power_right;
}Token;

typedef struct{
    Token** tokens;
    int index;
    int length;
}TokenVector;

Token* create_token(char* token_str, TokenType type);
char* create_str(const char* str1, int length);
bool check_prefix(const char* text,const char* prefix);
const char* check_keywords(const char* text, const char** keywords);
int count_tokens(const char* text, const char** keywords);
TokenVector* tokenizer(const char* text,const char** keywords);
