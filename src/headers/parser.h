#pragma once
#include "lexer.h"
#include "grammer.h"
#include "ctype.h"

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
        const Token* token;
        ExpressionList* expressions;
    };
    Expression(const Token* token,const char* identifier);
    Expression(ExpressionList* expressions, const char* identifier);
    ~Expression();
    void log(int depth);
    bool match_expression(GrammerRule* grammer_rule);
};

struct ExpressionList{
    Expression** expressions;
    int length;
    ExpressionList(TokenList* tokens, GrammerRuleList* grammer_rules);
    ExpressionList(Expression** exprs, int count);
    void reduce(GrammerRuleList* grammer_rules);
    void log();
    ~ExpressionList();
};
