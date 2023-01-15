
#include <math.h>

#include <debug.h>

#include "rotate.h"

struct mat4 mat4_rotateZ(
	float rad)
{
	return (struct mat4) {
		cosf(rad), -sinf(rad), 0, 0,
		sinf(rad),  cosf(rad), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1,
	};
}
