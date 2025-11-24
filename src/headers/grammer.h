#pragma once
#include "lexer.h"

enum GrammerRuleType{
    TOKEN,
    LIST,
};

struct GrammerRule{
    const char* ExprIdentifier;
    GrammerRuleType type;
    union{
        const Token* token;
        struct{
            const char** ExprIdentifiers;
            int length;
        };
    };
    GrammerRule(const char* expr_identifier, Token* token);
    GrammerRule(const char* expr_identifier, const char** ExprIdentifiers, int count);
    void log();
};

struct GrammerRuleList{
    int length;
    GrammerRule** rules;
    GrammerRuleList(GrammerRule** _rules, int _length);
    ~GrammerRuleList();
    void log();
};