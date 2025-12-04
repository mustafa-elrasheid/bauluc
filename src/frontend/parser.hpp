#pragma once
#include "lexer.hpp"
#include "grammer.hpp"
#include "ctype.h"
#include "../headers/utils.hpp"

enum ExpressionType{
    COMP,
    UNARY
};

struct ExpressionList;

struct Expression{
    ExpressionType type;
    const char* identifier;
    GrammerRule* matched_rule;
    union{
        const Lexer::Token* token;
        ExpressionList* expressions;
    };
    Expression(const Lexer::Token* token,const char* identifier);
    Expression(ExpressionList* expressions, const char* identifier);
    void log(int depth);
    bool match_expression(GrammerRule* grammer_rule);
    ~Expression();
};

struct ExpressionList{
    Expression** expressions;
    int length;
    ExpressionList(Lexer::TokenList* tokens, GrammerRuleList* grammer_rules);
    ExpressionList(Expression** exprs, int count);
    void reduce(GrammerRuleList* grammer_rules);
    void log();
    ~ExpressionList();
};
