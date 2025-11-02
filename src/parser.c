#include "parser.h"

Token* next(TokenVector* tokensvec){
    Token* value = tokensvec->tokens[tokensvec->index];
    tokensvec->index++;
    return value;
}

Token* peek(TokenVector* tokensvec){
    return tokensvec->tokens[tokensvec->index];
}

Token* parse_expression(TokenVector* tokens,int min_bp){
    Token* expr1 = next(tokens);
    switch (expr1->type) {
        case ATOM:
            printf("atom: %s\n",expr1->token);
            break;
        default:
            printf("bad token: %s\n",expr1->token);
    };
    while(1) {
        Token* op = peek(tokens);
        switch (op->type) {
            case END_OF_FILE:
                printf("end of file\n");
                goto end;
                break;
            case OPERATOR:
                printf("operator: %s\n",op->token);
                break;
            default:
                printf("bad token: %s\n",expr1->token);
        };
        
        int bind_power_left = op->binding_power_left, bind_power_right = op->binding_power_right;
        if (bind_power_left < min_bp)
            break;
        
        //In the video, `lexer.next()` is called BEFORE the if statement.
        //It must be called AFTER the precedence check, because calling it too early
        //would consume a token that shouldn't be parsed yet—leading to incorrect parse trees.
        next(tokens);
        Token* rhs = parse_expression(tokens, bind_power_right);
        // TODO: problem might be here lol
        expr1 = malloc(sizeof(Token));
        expr1->type = EXPRESSION;
        expr1->expr1 = expr1;
        expr1->expr2 = rhs;
    }
    end:
    return expr1;
}

void show_tree(Token* node,int depth){
    Token* expr1 = NULL;
    Token* expr2 = NULL;
    switch (node->type) {
        case EXPRESSION:
            printf("(\n");
            expr1 = node->expr1;
            expr2 = node->expr2;
            show_tree(expr1,depth+1);
            show_tree(expr2,depth+1);
            printf(")\n");
            break;
        case ATOM:
            printf("%s\n",node->token);
            break;
        case OPERATOR:
            printf("%s\n",node->token);
            break;
        case END_OF_FILE:
            printf("EOF\n");
            break;
        default:
            break;
    };
    
}