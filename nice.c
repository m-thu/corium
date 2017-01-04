/*
 * Invocation: nice [-n NICEVALUE] COMMAND [ARGS ...]
 *
 * Run COMMAND with NICEVALUE added to priority.
 *
 * Options:
 *   -n : nice increment (default: 10)
 *
 * Return value:
 *   1: failure
 *
 */

#include "lib.h"

int main(int argc, char *argv[])
{
	int prio = 10;

	if (argc == 1)
		return EXIT_FAILURE;

	if (argv[1][0] == '-' && argv[1][1] == 'n' && argv[1][2] == '\0') {
		/* exit on missing NICEVALUE and COMMAND */
		if (argc < 4)
			return EXIT_FAILURE;

		prio = atoi(argv[2]);
		argv += 2;
	}

	++argv;

	errno = 0;

	if (nice(prio) == -1 && errno != 0) {
		if (errno == EPERM)
			write_stderr("permission denied\n");
		return EXIT_FAILURE;
	}

	if (execvp(*argv, argv) < 0)
		return EXIT_FAILURE;

	/* suppress compiler warning */
	return EXIT_SUCCESS;
}
