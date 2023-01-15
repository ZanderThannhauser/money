
#include <string.h>

#include <debug.h>

#include <memory/smalloc.h>

#include "struct.h"
#include "new.h"

struct variable* new_variable(
	const char* name,
	const struct value value)
{
	ENTER;
	
	struct variable* this = smalloc(sizeof(*this));
	
	this->name = strdup(name);
	
	mpq_init(this->value.dollar);
	
	mpq_set(this->value.dollar, value.dollar);
	
	EXIT;
	return this;
}

