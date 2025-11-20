#include "lexer.h"

enum ExpressionType{
    BIN_EXPR,
    UNARY
};

struct Expression{
    ExpressionType type;
    union{
        const Token* token;
        struct{
            const Token* Operator;
            Expression* expr1;
            Expression* expr2;
        };
    };
    Expression(TokenVector* tokens,int min_bp);
    Expression(const Token* token);
    ~Expression();
    void log(int depth);
};

