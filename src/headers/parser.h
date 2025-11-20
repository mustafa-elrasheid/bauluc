#include "lexer.h"

enum ExpressionType{
    BIN_EXPR,
    UNARY
};

struct Expression{
    ExpressionType type;
    union{
        Token* token;
        struct{
            Token* Operator;
            Expression* expr1;
            Expression* expr2;
        };
    };
    Expression(TokenVector* tokens,int min_bp);
    Expression(Token* token);
    ~Expression();
    void show_tree(int depth);
};

