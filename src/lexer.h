#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

typedef enum{
    ATOM,
    OPERATOR,
    EXPRESSION,
    END_OF_FILE
}TokenType;

typedef struct{
    char* token;
    TokenType type;
    uint8_t binding_power_left;
    uint8_t binding_power_right;
    struct Token* expr1;
    struct Token* expr2;
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
