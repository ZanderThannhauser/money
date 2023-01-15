
#include <debug.h>

#include <memory/smalloc.h>

#include <avl/alloc_tree.h>

#include "variable/compare.h"
#include "variable/free.h"

#include "struct.h"
#include "new.h"

struct scope* new_scope()
{
	ENTER;
	
	struct scope* this = smalloc(sizeof(*this));
	
	this->tree = avl_alloc_tree(compare_variable, free_variable);
	
	EXIT;
	return this;
}

