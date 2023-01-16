
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdio.h>

struct zebu_token
{
	unsigned char* data;
	unsigned len, refcount, line;
};

struct zebu_$start
{
	struct {
		struct zebu_statement** data;
		unsigned n, cap;
	} statements;
	unsigned refcount, startline, endline;
};

struct zebu_additive_expression
{
	struct zebu_multiplicative_expression* inner;
	struct zebu_additive_expression* left;
	struct zebu_token* minus;
	struct zebu_token* plus;
	struct zebu_multiplicative_expression* right;
	unsigned refcount, startline, endline;
};

struct zebu_assignment
{
	struct zebu_expression* expression;
	struct zebu_token* name;
	unsigned refcount, startline, endline;
};

struct zebu_expression
{
	struct zebu_additive_expression* sub;
	unsigned refcount, startline, endline;
};

struct zebu_multiplicative_expression
{
	struct zebu_token* divide;
	struct zebu_prefix_expression* inner;
	struct zebu_multiplicative_expression* left;
	struct zebu_prefix_expression* right;
	struct zebu_token* times;
	unsigned refcount, startline, endline;
};

struct zebu_postfix_expression
{
	struct zebu_token* identifier;
	struct zebu_token* literal;
	struct zebu_expression* subexpression;
	unsigned refcount, startline, endline;
};

struct zebu_prefix_expression
{
	struct zebu_postfix_expression* inner;
	struct zebu_token* minus;
	struct zebu_token* plus;
	struct zebu_prefix_expression* sub;
	unsigned refcount, startline, endline;
};

struct zebu_statement
{
	struct zebu_assignment* assignment;
	struct zebu_transaction* transaction;
	unsigned refcount, startline, endline;
};

struct zebu_transaction
{
	struct zebu_postfix_expression* day;
	struct zebu_expression* delta;
	struct zebu_postfix_expression* hour;
	struct zebu_postfix_expression* minute;
	struct zebu_postfix_expression* month;
	struct zebu_postfix_expression* second;
	struct zebu_postfix_expression* year;
	unsigned refcount, startline, endline;
};



extern struct zebu_token* inc_zebu_token(struct zebu_token* token);
extern struct zebu_$start* inc_zebu_$start(struct zebu_$start* ptree);
extern struct zebu_additive_expression* inc_zebu_additive_expression(struct zebu_additive_expression* ptree);
extern struct zebu_assignment* inc_zebu_assignment(struct zebu_assignment* ptree);
extern struct zebu_expression* inc_zebu_expression(struct zebu_expression* ptree);
extern struct zebu_multiplicative_expression* inc_zebu_multiplicative_expression(struct zebu_multiplicative_expression* ptree);
extern struct zebu_postfix_expression* inc_zebu_postfix_expression(struct zebu_postfix_expression* ptree);
extern struct zebu_prefix_expression* inc_zebu_prefix_expression(struct zebu_prefix_expression* ptree);
extern struct zebu_statement* inc_zebu_statement(struct zebu_statement* ptree);
extern struct zebu_transaction* inc_zebu_transaction(struct zebu_transaction* ptree);


extern void free_zebu_token(struct zebu_token* token);
extern void free_zebu_$start(struct zebu_$start* ptree);

extern void free_zebu_additive_expression(struct zebu_additive_expression* ptree);

extern void free_zebu_assignment(struct zebu_assignment* ptree);

extern void free_zebu_expression(struct zebu_expression* ptree);

extern void free_zebu_multiplicative_expression(struct zebu_multiplicative_expression* ptree);

extern void free_zebu_postfix_expression(struct zebu_postfix_expression* ptree);

extern void free_zebu_prefix_expression(struct zebu_prefix_expression* ptree);

extern void free_zebu_statement(struct zebu_statement* ptree);

extern void free_zebu_transaction(struct zebu_transaction* ptree);



struct zebu_$start* zebu_parse(FILE* stream);
