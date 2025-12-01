#include "headers/grammer.hpp"

using namespace lexer;

GrammerRule::GrammerRule(const char* expr_identifier, Token* token){
    this->identifier = expr_identifier;
    this->token = token;
    this->type = TOKEN;
    this->length = 0;
}

GrammerRule::GrammerRule(const char* expr_identifier, const char** expr_identifiers, int count){
    this->identifier = expr_identifier;
    this->expr_identifiers = expr_identifiers;
    this->type = LIST;
    this->length = count;
}

GrammerRule::~GrammerRule(){
    if(this->type == TOKEN){
        delete this->token;
        return;
    }
    delete[] expr_identifiers;
}

void GrammerRule::log(){
    printf("rule: \"%s\", Indentifiers:",this->identifier);
    for(int x = 0; x < this->length && this->type == LIST; x++){
        printf("\"%s\", ",expr_identifiers[x]);
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
    delete[] this->rules;
}

void GrammerRuleList::log(){
    for(int x = 0; x < this->length; x++)
        this->rules[x]->log();
}