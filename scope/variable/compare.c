
#include <string.h>

#include <debug.h>

#include "struct.h"
#include "compare.h"

int compare_variable(
	const void* a,
	const void* b)
{
	const struct variable *A = a, *B = b;
	return strcmp(A->name, B->name);
}

