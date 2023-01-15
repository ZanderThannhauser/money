
#include <debug.h>

#include "translate.h"

struct mat4 mat4_translate(
	float x, float y, float z)
{
	return (struct mat4) {
		1, 0, 0, x,
		0, 1, 0, y,
		0, 0, 1, z,
		0, 0, 0, 1,
	};
}
