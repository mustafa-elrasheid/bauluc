#include "headers/parser.h"

Expression::Expression(const Token* token,const char* identifier){
    this->token = token;
    this->type = UNARY;
    this->identifier = identifier;
}

Expression::Expression(ExpressionList* expressions, const char* identifier){
    this->token = token;
    this->type = COMP;
    this->identifier = identifier;
    this->expressions = expressions;
}

Expression::~Expression(){
    if(this->type == UNARY) return;
    delete this->expressions;
}

void Expression::log(int depth){
    if(this->type == UNARY){
        for(int i = 0; i < depth; i++) printf("\t");
        printf("Token: \"%s\" (identifier: \"%s\")\n",(this)->token->token,this->identifier);
        return;
    }
    for(int i = 0; i < depth; i++) printf("\t");
    printf("%s, rule:",(this)->identifier);
    for(int x = 0; x < this->matched_rule->length; x++)
        printf("\"%s\",",this->matched_rule->expr_identifiers[x]);
    printf(" ):\n");
    for(int x = 0; x < this->expressions->length; x++)
        this->expressions->expressions[x]->log(depth+1);
}


bool Expression::match_expression(GrammerRule* grammer_rule){
    if(grammer_rule->type == LIST || this->token->type != grammer_rule->token->type)return false;
    if(
        (this->token->type == OPERATOR && !strcmp(this->token->token, grammer_rule->token->token)) ||
        (this->token->type == ATOM && !strcmp("NUM", grammer_rule->token->token) && isdigit(this->token->token[0])) ||
        (this->token->type == ATOM && !strcmp("STR", grammer_rule->token->token) && !isdigit(this->token->token[0]))
    )return true;
    return false;
}

ExpressionList::ExpressionList(TokenList* tokens, GrammerRuleList* grammer_rules){
    this->expressions = (Expression**) malloc(sizeof(Expression*)*tokens->length);
    this->length = tokens->length-1;
    for(int x = 0; x < tokens->length-1; x++){
        Expression* expr = new Expression(tokens->tokens[x],"");
        for(int i = 0; i < grammer_rules->length; i++)
            if(expr->match_expression(grammer_rules->rules[i])){
                this->expressions[x] = expr;
                expr->identifier = grammer_rules->rules[i]->identifier;
                break;
            }
        if(expressions[x] != expr) printf("no token to expression match found token:\"%s\"\n",tokens->tokens[x]->token);
    }
}

ExpressionList::ExpressionList(Expression** exprs, int count){
    this->expressions = exprs;
    this->length = count;
}

ExpressionList::~ExpressionList(){
    for(int x = 0; x < this->length; x++)
        delete expressions[x];
    free(expressions);
}

void ExpressionList::reduce(GrammerRuleList* grammer_rules){
    for(int ii = 0; ii < grammer_rules->length; ii++){
        if(grammer_rules->rules[ii]->type == TOKEN) continue;
        for(int i = 0; i < this->length; i++){
            GrammerRule* rule = grammer_rules->rules[ii];
            Expression* expr = this->expressions[i];
            int expr_count = 0;
            if (rule->length > this->length-i) continue;
            for(int iii = 0; iii < rule->length; iii++){
                const char* keywords[4] = {"*","+","?",NULL};
                TokenList* rule_identifier = new TokenList(rule->expr_identifiers[iii],keywords);
                int old_expr_count = expr_count;
                switch(rule_identifier->tokens[1]->token[0]){
                    case '?':
                        if(strcmp(
                            rule_identifier->tokens[0]->token,
                            this->expressions[i+(expr_count)]->identifier) == 0
                        )expr_count++;
                        delete rule_identifier;
                        continue;
                    case '+':
                        for(;strcmp(
                            rule_identifier->tokens[0]->token,
                            this->expressions[i+(expr_count)]->identifier) == 0
                        ;expr_count++);
                        delete rule_identifier;
                        if (old_expr_count == expr_count)break;
                        continue;
                    case '*':
                        for(;strcmp(
                            rule_identifier->tokens[0]->token,
                            this->expressions[i+(expr_count)]->identifier) == 0
                        ;expr_count++);
                        delete rule_identifier;
                        continue;
                    case 'E':
                        if(strcmp(
                            rule_identifier->tokens[0]->token,
                            this->expressions[i+(expr_count)]->identifier) != 0
                        ){delete rule_identifier; break;}
                        expr_count++;
                        delete rule_identifier;
                        continue;
                }
                expr_count=0;
                break;
            }
            if(expr_count==0) continue;
            Expression** exprs = (Expression**) malloc(sizeof(Expression*)*expr_count);
            memcpy(exprs, &this->expressions[i], sizeof(Expression*)*expr_count);
            this->length -= (expr_count-1);
            this->expressions[i] = new Expression(new ExpressionList(exprs, expr_count), rule->identifier);
            this->expressions[i]->matched_rule = rule;
            memcpy(
                &this->expressions[i+1],
                &this->expressions[i+expr_count],
                sizeof(Expression*)*(this->length-i-1)
            );
            ii=0;
            i=0;
        }
    }
}

void ExpressionList::log(){
    for(int i = 0; i < this->length; i++)
        this->expressions[i]->log(0);
}