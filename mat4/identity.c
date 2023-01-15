
#include <debug.h>

#include "identity.h"

struct mat4 mat4_identity()
{
	return (struct mat4) {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
}

