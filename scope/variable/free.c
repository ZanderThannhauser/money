
#include <stdlib.h>

#include <debug.h>

#include "struct.h"
#include "free.h"

void free_variable(
	void* ptr)
{
	ENTER;
	
	struct variable* this = ptr;
	
	free(this->name);
	
	mpz_clear(this->value.cents);
	
	free(this);
	
	EXIT;
}

