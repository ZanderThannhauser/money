
#include <structs/value.h>

struct zebu_expression;
struct scope;

struct value evaluate(
	struct zebu_expression* expression,
	struct scope* scope);
	
struct value evaluate_postfix(
	struct zebu_postfix_expression* postfix,
	struct scope* scope);
