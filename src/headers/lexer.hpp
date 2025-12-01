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

namespace lexer{


    enum TokenType{
        ATOM,
        OPERATOR,
        END_OF_FILE
    };

    struct Token{
        char* content;
        TokenType type;
        int binding_power_left;
        int binding_power_right;
        Token(const char* token_str, TokenType type);
        ~Token();
    };

    struct TokenList{
        Token** tokens;
        int index;
        int length;
        TokenList(const char* text,const char** keywords);
        ~TokenList();
        const Token* next();
        const Token* peek();
        void log();
        Token*& operator[](std::size_t i) {
            return tokens[i];
        }
        Token* operator[](std::size_t i) const {
            return tokens[i];
        }
        void flip_to_operator(const char** keywords);
        void remove_whitespace();
        void offside(const char* whitespace, const char* indent, const char* dedent);
        void insert(Token* token, int index);
    };
}