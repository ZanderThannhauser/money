
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
	
	dpvs(token->data);
	
	unsigned char* dot = memchr(token->data, '.', token->len);
	
	if (dot)
	{
		TODO;
		#if 0
		*dot = 0;
		
		mpq_t dollar, dollar;
		
		mpq_init_set_str(dollar, (char*) token->data, 10);
		
		mpq_init_set_str(dollar, (char*) dot + 1, 10);
		
		mpq_init(retval.dollar);
		
		mpq_mul_ui(retval.dollar, dollar, 100);
		
		mpq_add(retval.dollar, retval.dollar, dollar);
		
		mpq_clear(dollar), mpq_clear(dollar);
		#endif
	}
	else
	{
		mpq_init(retval.dollar);
		
		mpq_set_str(retval.dollar, (char*) token->data, 10);
		
/*		mpq_mul_ui(retval.dollar, retval.dollar, 100);*/
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
	else if (postfix->floor)
	{
		struct value inner = evaluate(postfix->subexpression, scope);
		
		mpz_t floor;
		
		mpz_init(floor);
		
		mpz_fdiv_q(floor, mpq_numref(inner.dollar), mpq_denref(inner.dollar));
		
		mpq_init(retval.dollar);
		
		mpq_set_z(retval.dollar, floor);
		
		mpq_clear(inner.dollar);
		
		mpz_clear(floor);
	}
	else if (postfix->ceil)
	{
		struct value inner = evaluate(postfix->subexpression, scope);
		
		mpz_t ceil;
		
		mpz_init(ceil);
		
		mpz_cdiv_q(ceil, mpq_numref(inner.dollar), mpq_denref(inner.dollar));
		
		mpq_init(retval.dollar);
		
		mpq_set_z(retval.dollar, ceil);
		
		mpq_clear(inner.dollar);
		
		mpz_clear(ceil);
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
		
		mpq_neg(retval.dollar, retval.dollar);
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
		TODO;
		#if 0
		struct value left = evaluate_multiplicative(additive->left, scope);
		
		struct value right = evaluate_prefix(additive->right, scope);
		
		mpq_init(retval.dollar);
		
		mpq_mul(retval.dollar, left.dollar, right.dollar);
		
		mpq_fdiv_q_ui(retval.dollar, retval.dollar, 100);
		
		mpq_clear(left.dollar), mpq_clear(right.dollar);
		#endif
	}
	else if (additive->divide)
	{
		struct value left = evaluate_multiplicative(additive->left, scope);
		
		struct value right = evaluate_prefix(additive->right, scope);
		
		mpq_init(retval.dollar);
		
		mpq_div(retval.dollar, left.dollar, right.dollar);
		
		mpq_clear(left.dollar), mpq_clear(right.dollar);
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
		TODO;
		#if 0
		struct value left = evaluate_additive(additive->left, scope);
		
		struct value right = evaluate_multiplicative(additive->right, scope);
		
		mpq_init(retval.dollar);
		
		mpq_add(retval.dollar, left.dollar, right.dollar);
		
		mpq_clear(left.dollar), mpq_clear(right.dollar);
		#endif
	}
	else if (additive->minus)
	{
		TODO;
		#if 0
		struct value left = evaluate_additive(additive->left, scope);
		
		struct value right = evaluate_multiplicative(additive->right, scope);
		
		mpq_init(retval.dollar);
		
		mpq_sub(retval.dollar, left.dollar, right.dollar);
		
		mpq_clear(left.dollar), mpq_clear(right.dollar);
		#endif
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
	
	struct value retval = evaluate_additive(expression->sub, scope);
	
	EXIT;
	return retval;
}













