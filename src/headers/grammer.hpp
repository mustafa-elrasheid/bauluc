#pragma once
#include "lexer.hpp"

enum GrammerRuleType{
    TOKEN,
    LIST,
};

struct GrammerRule{
    const char* identifier;
    GrammerRuleType type;
    union{
        lexer::Token* token;
        struct{
            const char** expr_identifiers;
            int length;
        };
    };
    GrammerRule(const char* expr_identifier, lexer::Token* token);
    GrammerRule(const char* expr_identifier, const char** expr_identifiers, int count);
    ~GrammerRule();
    void log();
};

struct GrammerRuleList{
    int length;
    GrammerRule** rules;
    GrammerRuleList(GrammerRule** _rules, int _length);
    ~GrammerRuleList();
    void log();
};