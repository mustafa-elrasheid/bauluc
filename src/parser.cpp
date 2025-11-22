#include "headers/parser.h"

Expression::Expression(const Token* token,const char* Identifier){
    this->token = token;
    this->type = UNARY;
    this->Identifier = Identifier;
}

Expression::Expression(ExpressionList* expressions, const char* Identifier){
    this->token = token;
    this->type = COMP;
    this->Identifier = Identifier;
    this->expressions = expressions;
}

Expression::~Expression(){
    if(this->type == UNARY) return;
    delete this->expressions;
}

void Expression::log(int depth){
    if(this->type == UNARY){
        for(int i = 0; i < depth; i++) printf("\t");
        printf("Token: \"%s\" (Identifier:\"%s\")\n",(this)->token->token,this->Identifier);
        return;
    }
    for(int i = 0; i < depth; i++) printf("\t");
    printf("Expression (Identifier: \"%s\"):\n",(this)->Identifier);
    for(int x = 0; x < this->expressions->length; x++)
        this->expressions->expressions[x]->log(depth+1);
}


bool Expression::match_expression(GrammerRule* grammer_rule){
    if(grammer_rule->type == LIST)return false;
    if(this->token->type != grammer_rule->token->type) return false;
    if(this->token->type == OPERATOR && !strcmp(this->token->token, grammer_rule->token->token))return true;
    if(this->token->type == ATOM && !strcmp("NUM", grammer_rule->token->token) && isdigit(this->token->token[0]))return true;
    if(this->token->type == ATOM && !strcmp("STR", grammer_rule->token->token) && isdigit(this->token->token[0]))return true;
    return false;
}

ExpressionList::ExpressionList(TokenList* tokens, GrammerRuleList* grammer_rules){
    this->expressions = (Expression**) malloc(sizeof(Expression*)*tokens->length);
    this->length = tokens->length-1;
    for(int x = 0; x < tokens->length; x++){
        Expression* expr = new Expression(tokens->tokens[x],"");
        for(int i = 0; i < grammer_rules->length; i++){
            if(expr->match_expression(grammer_rules->rules[i])){
                this->expressions[x] = expr;
                expr->Identifier = grammer_rules->rules[i]->ExprIdentifier;
                break;
            }
        }
    }
}

ExpressionList::ExpressionList(Expression** exprs, int count){
    this->expressions = exprs;
    this->length = count;
}

void ExpressionList::reduce(GrammerRuleList* grammer_rules){
    for(int ii = 0; ii < grammer_rules->length; ii++){
        GrammerRule* rule = grammer_rules->rules[ii];
        for(int i = 0; i < this->length; i++){
            Expression* expr = this->expressions[i];
            int ix = 0;
            if (rule->type == TOKEN) continue;
            if (length-i < rule->length) continue;
            bool rule_matched = true;
            do{
                bool increament = true;
                for(int iii = 0; iii < rule->length; iii++)
                    if(strcmp(
                        rule->ExprIdentifiers[iii],
                        this->expressions[i+iii+(rule->length*ix)]->Identifier) != 0
                    ){rule_matched = true;increament = false;}
                if(increament)ix++;
            }while(rule->repeat && (rule->length*ix)+i < this->length && rule_matched != true);
            if(ix==0)continue;
            Expression** exprs = (Expression**) malloc(sizeof(Expression*)*rule->length*ix);
            memcpy(exprs,this->expressions+i,sizeof(Expression*)*rule->length*ix);
            Expression* reduced_expr = new Expression(
                new ExpressionList(exprs,rule->length*ix),
                rule->ExprIdentifier
            );
            this->length -= ((rule->length*ix)-1);
            this->expressions[i] = reduced_expr;
            memcpy(
                this->expressions+i+1,
                this->expressions+i+(rule->length*ix),
                sizeof(Expression*)*(this->length-i-1)
            );
        }
    }
}

void ExpressionList::log(){
    for(int i = 0; i < this->length; i++){
        this->expressions[i]->log(0);
    }
}