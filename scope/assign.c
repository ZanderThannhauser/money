
#include <assert.h>

#include <debug.h>

#include <avl/search.h>
#include <avl/insert.h>

#include "variable/struct.h"
#include "variable/new.h"

#include "struct.h"
#include "assign.h"

void scope_assign(
	struct scope* this,
	const char* name,
	struct value value)
{
	ENTER;
	
	dpvs(name);
	
	struct avl_node_t* node = avl_search(this->tree, &name);
	
	if (node)
	{
		struct variable* variable = node->item;
		
		mpq_set(variable->value.dollar, value.dollar);
	}
	else
	{
		struct variable* variable = new_variable(name, value);
		
		void* ptr = avl_insert(this->tree, variable);
		
		assert(ptr);
	}
	
	EXIT;
}

