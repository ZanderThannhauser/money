
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
		*dot = 0;
		
		mpq_init(retval.dollar);
		
		mpq_set_str(retval.dollar, (char*) token->data, 10);
		
		mpq_t decimal, ten;
		
		mpq_init(decimal);
		mpq_init(ten);
		
		mpq_set_ui(decimal, 1, 10);
		mpq_set_ui(ten, 10, 1);
		
		for (unsigned char* m = dot + 1; *m; m++)
		{
			dpvc(*m);
			
			for (unsigned char x = *m; x > '0'; x--)
				mpq_add(retval.dollar, retval.dollar, decimal);
			
			mpq_div(decimal, decimal, ten);
		}
		
		mpq_clear(decimal);
		mpq_clear(ten);
	}
	else
	{
		mpq_init(retval.dollar);
		
		mpq_set_str(retval.dollar, (char*) token->data, 10);
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
		struct value left = evaluate_multiplicative(additive->left, scope);
		
		struct value right = evaluate_prefix(additive->right, scope);
		
		mpq_init(retval.dollar);
		
		mpq_mul(retval.dollar, left.dollar, right.dollar);
		
		mpq_clear(left.dollar), mpq_clear(right.dollar);
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
		struct value left = evaluate_additive(additive->left, scope);
		
		struct value right = evaluate_multiplicative(additive->right, scope);
		
		mpq_init(retval.dollar);
		
		mpq_add(retval.dollar, left.dollar, right.dollar);
		
		mpq_clear(left.dollar), mpq_clear(right.dollar);
	}
	else if (additive->minus)
	{
		struct value left = evaluate_additive(additive->left, scope);
		
		struct value right = evaluate_multiplicative(additive->right, scope);
		
		mpq_init(retval.dollar);
		
		mpq_sub(retval.dollar, left.dollar, right.dollar);
		
		mpq_clear(left.dollar), mpq_clear(right.dollar);
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













