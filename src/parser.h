#include "lexer.h"

typedef enum{
    BIN_EXPR,
    UNARY
}ExpressionType;

typedef struct{
    ExpressionType type;
}Expression;

typedef struct{
    ExpressionType type;
    Token* token;
}UniExpression;

typedef struct{
    ExpressionType type;
    Token* Operator;
    Expression* expr1;
    Expression* expr2;
}BinaryExpression;

Token* next(TokenVector* tokensvec);
Token* peek(TokenVector* tokensvec);
Expression* parse_expression(TokenVector* tokens,int min_bp);
void free_expression(Expression* node);
void show_tree(Expression* node, int depth);