
#include <debug.h>

#include "vec4.h"

struct vec4 vec4(
	float x, float y, float z, float w)
{
	return (struct vec4) {
		x, y, z, w
	};
}
