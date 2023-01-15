
#include <stdlib.h>
#include <stdio.h>

#include <debug.h>

#include "usage.h"

void usage(int code)
{
	printf(""
		"usage: money [-vh] -i <path/to/input.money>\n"
	"");
	
	exit(code);
}


