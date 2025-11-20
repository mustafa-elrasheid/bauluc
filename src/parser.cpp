#include "headers/parser.h"

Expression::Expression(Token* token){
    this->token = token;
    this->type = UNARY;
}

Expression::Expression(TokenVector* tokens,int min_bp){
    Token* token1 = tokens->next();
    this->type = UNARY;
    this->token = token1;
    if(token1->type != ATOM) {
        printf("Unexpected token: %s\n", token1->token);
        return;
    }
    while(1){
        Token* op = tokens->peek();
        if(op->type != END_OF_FILE && op->type != OPERATOR){
            printf("bad token: %s\n",op->token);
            return;
        }
        if (op->binding_power_left < min_bp) return;
        tokens->next();
        Expression* rhs = new Expression(tokens, op->binding_power_right);
        if(rhs == NULL) return;
        // move rhs and this to this
        Expression* lhs = new Expression(*this);
        this->type = BIN_EXPR;
        this->expr1 = lhs;
        this->expr2 = rhs;
        this->Operator = op;
    }
}

Expression::~Expression(){
    switch (this->type) {
        case UNARY:
            break;
        case BIN_EXPR:
            delete this->expr1;
            delete this->expr2;
            break;
        default:
            break;
    };
}

void Expression::log(int depth){
    switch (this->type) {
        case UNARY:
            for(int i = 0; i < depth; i++) printf("  ");
            printf("%s\n",(this)->token->token);
            break;
        case BIN_EXPR:
            for(int i = 0; i < depth; i++) printf("  ");
            printf("Binary Expression (Operator: '%s'):\n",(this)->Operator->token);
            this->expr1->log(depth+1);
            this->expr2->log(depth+1);
            break;
        default:
            break;
    };
}