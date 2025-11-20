#include "headers/parser.h"

Expression::Expression(const Token* token){
    this->token = token;
    this->type = UNARY;
}

Expression::Expression(TokenVector* tokens,int min_bp){
    this->type = UNARY;
    this->token = tokens->next();
    if(this->token->type != ATOM) {
        printf("Unexpected token: %s\n", this->token->token);
        return;
    }
    while(1){
        const Token* op = tokens->peek();
        if(op->type != END_OF_FILE && op->type != OPERATOR){
            printf("bad token: %s\n",op->token);
            return;
        }
        if (op->binding_power_left < min_bp) return;
        tokens->next();
        Expression* rhs = new Expression(tokens, op->binding_power_right);
        if(rhs == NULL) return;
        Expression* lhs = new Expression(*this);
        this->type = BIN_EXPR;
        this->expr1 = lhs;
        this->expr2 = rhs;
        this->Operator = op;
    }
}

Expression::~Expression(){
    if(this->type != BIN_EXPR) return;
    delete this->expr1;
    delete this->expr2;
}

void Expression::log(int depth){
    if(this->type == UNARY){
        for(int i = 0; i < depth; i++) printf("  ");
        printf("%s\n",(this)->token->token);
        return;
    }
    for(int i = 0; i < depth; i++) printf("  ");
    printf("Binary Expression (Operator: '%s'):\n",(this)->Operator->token);
    this->expr1->log(depth+1);
    this->expr2->log(depth+1);
}