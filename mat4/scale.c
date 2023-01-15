
#include <debug.h>

#include "scale.h"

struct mat4 mat4_scale(
	double x,
	double y,
	double z,
	double w)
{
	return (struct mat4) {
		x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, z, 0,
		0, 0, 0, w,
	};
}
