/*
 * Invocation: nice [-n NICEVALUE] COMMAND [ARGS ...]
 *
 * Run COMMAND with priority -20 <= NICEVALUE <= 19
 *
 * Options:
 *   -n : priority (default: 10)
 *
 * Return value:
 *   1: failure
 *
 */

#include "lib.h"

#define DEFAULT_NICE_VALUE 10

int main(int argc, char *argv[])
{
	int prio = DEFAULT_NICE_VALUE;

	if (argc == 1)
		return EXIT_FAILURE;

	if (argv[1][0] == '-' && argv[1][1] == 'n' && argv[1][2] == '\0') {
		/* exit on missing NICEVALUE and COMMAND */
		if (argc < 4)
			return EXIT_FAILURE;

		prio = atoi(argv[2]);

		if (prio < PRIO_MIN || prio > PRIO_MAX)
			return EXIT_FAILURE;

		argv += 2;
	}

	++argv;

	errno = 0;

	if (nice(prio) == -1 && errno != 0) {
		if (errno == EPERM)
			write_stderr("permission denied\n");
		return EXIT_FAILURE;
	}

	if (execve(*argv, argv, environ) < 0)
		return EXIT_FAILURE;

	/* suppress compiler warning */
	return EXIT_SUCCESS;
}
