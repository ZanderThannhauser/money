
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <getopt.h>
#include <sys/time.h>

#include <debug.h>

/*#include <macros/strequals.h>*/

#include <enums/error.h>

#include "just_print.h"
#include "usage.h"
#include "verbose.h"
#include "input_path.h"

void cmdln_process(int argc, char* const* argv)
{
	ENTER;
	
	int opt, option_index;
	const struct option long_options[] = {
		{"input",                  required_argument, 0, 'i'},
		{"just-print",                   no_argument, 0, 'p'},
		{"verbose",                      no_argument, 0, 'v'},
		{"help",                         no_argument, 0, 'h'},
		{ 0,                                       0, 0,  0 },
	};
	
	while ((opt = getopt_long(argc, argv,
		"i:" "p" "v" "h",
		long_options, &option_index)) >= 0)
	{
		switch (opt)
		{
			case 'i':
				input_path = optarg;
				break;
			
			case 'p':
				just_print = true;
				break;
			
			case 'v':
				#ifdef VERBOSE
				verbose = true;
				#endif
				break;
			
			case 'h':
				usage(0);
				break;
			
			default:
			{
				fprintf(stderr, "zebu: unknown flag '%s'!\n", optarg);
				usage(e_bad_cmdline_args);
				break;
			}
		}
	}
	
	if (!input_path)
	{
		fprintf(stderr, "zebu: missing arguments!\n");
		usage(e_bad_cmdline_args);
	}
	
	dpvs(input_path);
	dpvb(just_print);
	dpvb(verbose);
	
	EXIT;
}
















