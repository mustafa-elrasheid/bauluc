#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

char* create_str(const char* str1, int length);
bool check_prefix(const char* text,const char* prefix);
const char* check_keywords(const char* text, const char** keywords);
int count_tokens(const char* text, const char** keywords);

enum TokenType{
    ATOM,
    OPERATOR,
    END_OF_FILE
};

struct Token{
    char* token;
    TokenType type;
    int binding_power_left;
    int binding_power_right;
    Token(char* token_str, TokenType type);
    ~Token();
};

struct TokenVector{
    Token** tokens;
    int index;
    int length;
    TokenVector(const char* text,const char** keywords);
    ~TokenVector();
    Token* next();
    Token* peek();
    void log();
};
