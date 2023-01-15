
#include <stdlib.h>

#include <assert.h>

#include <debug.h>

#include <avl/search.h>

#include "variable/struct.h"

#include "struct.h"
#include "lookup.h"

struct value scope_lookup(
	struct scope* this,
	const char* name)
{
	struct value retval;
	ENTER;
	
	dpvs(name);
	
	struct avl_node_t* node = avl_search(this->tree, &name);
	
	if (!node)
	{
		TODO;
		exit(1);
	}
	
	struct variable* variable = node->item;
	
	mpq_init(retval.dollar);
	
	mpq_set(retval.dollar, variable->value.dollar);
	
	EXIT;
	return retval;
}


