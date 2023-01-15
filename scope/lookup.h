
#include <structs/value.h>

struct scope;

struct value scope_lookup(
	struct scope* this,
	const char* name);

