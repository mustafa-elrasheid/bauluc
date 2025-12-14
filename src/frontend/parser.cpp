#include "parser.hpp"

Expression::Expression(const Lexer::Token* token, const char* identifier){
	this->token = token;
	this->type = UNARY;
	this->identifier = identifier;
}

Expression::Expression(ExpressionList* expressions, const char* identifier){
	this->token = token;
	this->type = COMP;
	this->identifier = identifier;
	this->expressions = *expressions;
}

Expression::~Expression(){
	if(this->type == UNARY) return;
}

void Expression::log(int depth){
	if(this->type == UNARY){
		for(int i = 0; i < depth; i++) printf("\t");
		printf("Token: \"%s\" (Content: \"%s\")\n", this->identifier,(this)->token->content);
		return;
	}
	for(int i = 0; i < depth; i++) printf("\t");
	printf("Expression: \"%s\" (Rule:", (this)->identifier);
	for(int x = 0; x < this->matched_rule->length; x++){
		printf("\"%s\"", this->matched_rule->expr_identifiers[x]);
		if(x != this->matched_rule->length-1) printf(", ");
	}
	printf(" )\n");
	for(int x = 0; x < this->expressions.length; x++)
		this->expressions.expressions[x]->log(depth+1);
}


bool Expression::match_expression(GrammerRule* grammer_rule){
	if(grammer_rule->type == LIST || this->token->type != grammer_rule->token->type) return false;
	if(this->token->type == Lexer::OPERATOR && !strcmp(this->token->content, grammer_rule->token->content)) return true;
	if(this->token->type == Lexer::ATOM     && !strcmp("NUM", grammer_rule->token->content) && isdigit(this->token->content[0])) return true;
	if(this->token->type == Lexer::ATOM     && !strcmp("IDF", grammer_rule->token->content) && !isdigit(this->token->content[0])) return true;
	if(this->token->type == Lexer::OPERATOR && !strcmp("STR", grammer_rule->token->content) && this->token->content[0] == '\"') return true;
	return false;
}

ExpressionList::ExpressionList(Lexer::TokenList* tokens, GrammerRuleList* grammer_rules){
	this->expressions = (Expression**) malloc(sizeof(Expression*)*tokens->length);
	this->length = tokens->length-1;
	for(int x = 0; x < tokens->length-1; x++){
		Expression* expr = new Expression(tokens->tokens[x], "");
		for(int i = 0; i < grammer_rules->length; i++)
			if(expr->match_expression(grammer_rules->rules[i])){
				this->expressions[x] = expr;
				expr->identifier = grammer_rules->rules[i]->identifier;
				break;
			}
		if(expressions[x] != expr) error("Unknown Token","Unexpected Token: \"%s\"\n",expr->token->content);
	}
}

ExpressionList::ExpressionList(Expression** exprs, int count){
	this->expressions = exprs;
	this->length = count;
}

ExpressionList::ExpressionList(){
	this->expressions = NULL;
	this->length = 0;
}

ExpressionList::~ExpressionList(){
	for(int x = 0; x < this->length; x++)
		delete expressions[x];
	free(expressions);
}

Expression*& ExpressionList::operator[](int i){
	return expressions[i];
}

Expression*  ExpressionList::operator[](int i) const {
	return expressions[i];
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
				Lexer::TokenList rule_identifier = Lexer::TokenList(rule->expr_identifiers[iii], keywords);
				int old_expr_count = expr_count;
				switch(rule_identifier[1]->content[0]){
					case '?':
						if(strcmp(
							rule_identifier[0]->content,
							this->expressions[i+(expr_count)]->identifier) == 0
						)expr_count++;
						continue;
					case '+':
						for(;strcmp(
							rule_identifier[0]->content,
							this->expressions[i+(expr_count)]->identifier) == 0
						;expr_count++);
						if (old_expr_count == expr_count)break;
						continue;
					case '*':
						for(;strcmp(
							rule_identifier[0]->content,
							this->expressions[i+(expr_count)]->identifier) == 0
						;expr_count++);
						continue;
					case 'E':
						if(strcmp(
							rule_identifier[0]->content,
							this->expressions[i+(expr_count)]->identifier) != 0
						)break;
						expr_count++;
						continue;
				}
				expr_count = 0;
				break;
			}
			if(expr_count == 0) continue;
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
			ii = 0;
			i = 0;
		}
	}
}

void ExpressionList::log(){
	for(int i = 0; i < this->length; i++)
		this->expressions[i]->log(0);
}