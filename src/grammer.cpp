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

void GrammerRule::log(){
    printf("rule: \"%s\", Indentifiers:",this->ExprIdentifier);
    for(int x = 0; x < this->length && this->type == LIST; x++){
        printf("\"%s\", ",ExprIdentifiers[x]);
    }
    printf("\n");
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

void GrammerRuleList::log(){
    for(int x = 0; x < this->length; x++)
        this->rules[x]->log();
}