#pragma once
#include "lexer.hpp"

enum GrammerRuleType{
	TOKEN,
	LIST,
};

struct GrammerRule{
	const char* identifier;
	GrammerRuleType type;
	union{
		Lexer::Token* token;
		struct{
			const char** expr_identifiers;
			int length;
		};
	};
	GrammerRule(const char* expr_identifier, Lexer::Token* token);
	GrammerRule(const char* expr_identifier, const char** expr_identifiers, int count);
	void log();
	~GrammerRule();
};

struct GrammerRuleList{
	int length;
	GrammerRule** rules;
	GrammerRuleList(GrammerRule** _rules, int _length);
	void log();
	~GrammerRuleList();
};