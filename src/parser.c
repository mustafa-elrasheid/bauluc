#include "parser.h"

Token* next(TokenVector* tokensvec){
    Token* value = tokensvec->tokens[tokensvec->index];
    tokensvec->index++;
    return value;
}

Token* peek(TokenVector* tokensvec){
    return tokensvec->tokens[tokensvec->index];
}

Expression* parse_expression(TokenVector* tokens,int min_bp){
    Token* token1 = next(tokens);
    UniExpression* expr1 = malloc(sizeof(UniExpression));
    expr1->type = UNARY;
    expr1->token = token1;
    if(token1->type != ATOM) {
        printf("Unexpected token: %s\n", token1->token);
        return NULL;
    }
    while(1){
        Token* op = peek(tokens);
        if(op->type != END_OF_FILE && op->type != OPERATOR){
            printf("bad token: %s\n",op->token);
            return NULL;
        }
        if (op->binding_power_left < min_bp) return expr1;
        next(tokens);
        Expression* rhs = parse_expression(tokens, op->binding_power_right);
        if(rhs == NULL) return NULL;
        Expression* temp_expr1 = expr1;
        expr1 = malloc(sizeof(BinaryExpression));
        expr1->type = BIN_EXPR;
        ((BinaryExpression*)expr1)->expr1 = (Expression*)temp_expr1;
        ((BinaryExpression*)expr1)->expr2 = rhs;
        ((BinaryExpression*)expr1)->Operator = op;
    }
    return expr1;
}

void free_expression(Expression* node){
    switch (node->type) {
        case UNARY:
            free(node);
            break;
        case BIN_EXPR:
            free_expression(((BinaryExpression*)node)->expr1);
            free_expression(((BinaryExpression*)node)->expr2);
            free(node);
            break;
        default:
            break;
    };
}

void show_tree(Expression* node,int depth){
    switch (node->type) {
        case UNARY:
            for(int i = 0; i < depth; i++) printf("  ");
            printf("%s\n",((UniExpression*)node)->token->token);
            break;
        case BIN_EXPR:
            for(int i = 0; i < depth; i++) printf("  ");
            printf("Binary Expression (Operator: '%s'):\n",((BinaryExpression*)node)->Operator->token);
            show_tree(((BinaryExpression*)node)->expr1,depth+1);
            show_tree(((BinaryExpression*)node)->expr2,depth+1);
            break;
        default:
            break;
    };
}