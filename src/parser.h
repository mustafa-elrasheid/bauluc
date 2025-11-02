#include "lexer.h"

Token* next(TokenVector* tokensvec);
Token* peek(TokenVector* tokensvec);
Token* parse_expression(TokenVector* tokens,int min_bp);
void show_tree(Token* node, int depth);