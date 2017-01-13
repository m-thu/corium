/*
 * Invocation: env [OPTION] [VARIABLE=VALUE [...]] [COMMAND [ARG [...]]] 
 *
 * Run COMMAND with modified environment, no COMMAND: display environment.
 *
 * Options:
 *   -i : empty environment
 *
 * Return value:
 *   0: success
 *   1: failure
 *
 */

#include "lib.h"

int main(int argc, char *argv[])
{
	(void)argc;

	if (*++argv && argv[0][0] == '-' && argv[0][1] == 'i'
	    && argv[0][2] == '\0') {
		*(char **)environ = NULL;
		++argv;
	}

	for (; *argv; ++argv) {
		if (strchr(*argv, '=')) {
			putenv(*argv);
		} else {
			execvp(*argv, argv);
			return EXIT_FAILURE;
		}
	}

	for (; *environ; ++environ) {
		write_stdout(*environ);
		write(1, "\n", 1);
	}

	return EXIT_SUCCESS;
}
