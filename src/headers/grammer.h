#pragma once
#include "lexer.h"

enum GrammerRuleType{
    TOKEN,
    COMP
};

struct GrammerRule{
    const char* ExprIdentifier;
    GrammerRuleType type;
    union{
        const Token* token;
        const char** ExprIdentifiers;
    };
};