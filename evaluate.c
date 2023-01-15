
#include <assert.h>
#include <string.h>

#include <debug.h>

#include <zebu.h>

#include <scope/lookup.h>

#include "evaluate.h"

static struct value evaluate_literal(
	struct zebu_token* token)
{
	struct value retval;
	ENTER;
	
	unsigned char* dot = memchr(token->data, '.', token->len);
	
	if (dot)
	{
		*dot = 0;
		
		mpz_t dollar, cents;
		
		mpz_init_set_str(dollar, (char*) token->data, 10);
		
		mpz_init_set_str(cents, (char*) dot + 1, 10);
		
		mpz_init(retval.cents);
		
		mpz_mul_ui(retval.cents, dollar, 100);
		
		mpz_add(retval.cents, retval.cents, cents);
		
		mpz_clear(dollar), mpz_clear(cents);
	}
	else
	{
		mpz_init_set_str(retval.cents, (char*) token->data, 10);
		
		mpz_mul_ui(retval.cents, retval.cents, 100);
	}
	
	EXIT;
	return retval;
}

struct value evaluate_postfix(
	struct zebu_postfix_expression* postfix,
	struct scope* scope)
{
	struct value retval;
	ENTER;
	
	if (postfix->literal)
	{
		retval = evaluate_literal(postfix->literal);
	}
	else if (postfix->identifier)
	{
		retval = scope_lookup(scope, (const char*) postfix->identifier->data);
	}
	else if (postfix->subexpression)
	{
		retval = evaluate(postfix->subexpression, scope);
	}
	else
	{
		TODO;
	}
	
	EXIT;
	return retval;
}


static struct value evaluate_prefix(
	struct zebu_prefix_expression* additive,
	struct scope* scope)
{
	struct value retval;
	ENTER;
	
	if (additive->inner)
	{
		retval = evaluate_postfix(additive->inner, scope);
	}
	else if (additive->plus)
	{
		retval = evaluate_prefix(additive->sub, scope);
	}
	else if (additive->minus)
	{
		retval = evaluate_prefix(additive->sub, scope);
		
		mpz_neg(retval.cents, retval.cents);
	}
	else
	{
		TODO;
	}
	
	EXIT;
	return retval;
}


static struct value evaluate_multiplicative(
	struct zebu_multiplicative_expression* additive,
	struct scope* scope)
{
	struct value retval;
	ENTER;
	
	if (additive->inner)
	{
		retval = evaluate_prefix(additive->inner, scope);
	}
	else if (additive->times)
	{
		struct value left = evaluate_multiplicative(additive->left, scope);
		
		struct value right = evaluate_prefix(additive->right, scope);
		
		mpz_init(retval.cents);
		
		mpz_mul(retval.cents, left.cents, right.cents);
		
		mpz_fdiv_q_ui(retval.cents, retval.cents, 100);
		
		mpz_clear(left.cents), mpz_clear(right.cents);
	}
	else if (additive->divide)
	{
		struct value left = evaluate_multiplicative(additive->left, scope);
		
		struct value right = evaluate_prefix(additive->right, scope);
		
		mpz_mul_ui(left.cents, left.cents, 100);
		
		mpz_init(retval.cents);
		
		mpz_div(retval.cents, left.cents, right.cents);
		
		mpz_clear(left.cents), mpz_clear(right.cents);
	}
	else
	{
		TODO;
	}
	
	EXIT;
	return retval;
}


static struct value evaluate_additive(
	struct zebu_additive_expression* additive,
	struct scope* scope)
{
	struct value retval;
	ENTER;
	
	if (additive->inner)
	{
		retval = evaluate_multiplicative(additive->inner, scope);
	}
	else if (additive->plus)
	{
		struct value left = evaluate_additive(additive->left, scope);
		
		struct value right = evaluate_multiplicative(additive->right, scope);
		
		mpz_init(retval.cents);
		
		mpz_add(retval.cents, left.cents, right.cents);
		
		mpz_clear(left.cents), mpz_clear(right.cents);
	}
	else if (additive->minus)
	{
		struct value left = evaluate_additive(additive->left, scope);
		
		struct value right = evaluate_multiplicative(additive->right, scope);
		
		mpz_init(retval.cents);
		
		mpz_sub(retval.cents, left.cents, right.cents);
		
		mpz_clear(left.cents), mpz_clear(right.cents);
	}
	else
	{
		TODO;
	}
	
	EXIT;
	return retval;
}


static struct value evaluate_ternary(
	struct zebu_ternary_expression* ternary,
	struct scope* scope)
{
	struct value retval;
	ENTER;
	
	if (ternary->qmark)
	{
		TODO;
	}
	else if (ternary->inner)
	{
		retval = evaluate_additive(ternary->inner, scope);
	}
	else
	{
		TODO;
	}
	
	EXIT;
	return retval;
}

struct value evaluate(
	struct zebu_expression* expression,
	struct scope* scope)
{
	ENTER;
	
	struct value retval = evaluate_ternary(expression->sub, scope);
	
	EXIT;
	return retval;
}













