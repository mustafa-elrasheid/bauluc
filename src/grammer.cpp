#include "headers/grammer.h"

GrammerRule::GrammerRule(const char* expr_identifier, Token* token){
    this->ExprIdentifier = expr_identifier;
    this->token = token;
    this->type = TOKEN;
}

GrammerRule::GrammerRule(const char* expr_identifier, const char** ExprIdentifiers, int count){
    this->ExprIdentifier = expr_identifier;
    this->ExprIdentifiers = ExprIdentifiers;
    this->type = LIST;
    this->length = count;
}

GrammerRuleList::GrammerRuleList(GrammerRule** _rules, int _length){
    this->length = _length;
    this->rules = _rules;
}

GrammerRuleList::~GrammerRuleList(){
    for(int x = 0; x < length; x++)
        delete rules[x];
    delete this->rules;
}