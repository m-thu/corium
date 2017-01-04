/*
 * Invocation: renice [-n] INCREMENT PID [...]
 *
 * Renice the process with pid PID.
 *
 * Options:
 *   -n: if omitted, INCREMENT is an absolute value
 *
 * Return value:
 *   0: success
 *   1: failure
 *
 */

#include "lib.h"

int main(int argc, char *argv[])
{
	bool opt_n = false;

	if (argc == 1)
		return EXIT_FAILURE;

	++argv;

	if (argv[0][0] == '-' && argv[0][1] == 'n' && argv[0][2] == '\0') {
		opt_n = true;
		++argv;

		if (argc < 4)
			return EXIT_FAILURE;
	} else {
		if (argc < 3)
			return EXIT_FAILURE;
	}

	int prio = atoi(argv[0]);

	while (*++argv) {
		pid_t pid = atoi(*argv);

		if (opt_n) {
			int ret;

			errno = 0;

			if ((ret = getpriority(PRIO_PROCESS, pid)) == -1 && errno != 0)
				return EXIT_FAILURE;

			prio += ret;
		}

		if (prio < PRIO_MIN)
			prio = PRIO_MIN;
		if (prio > PRIO_MAX)
			prio = PRIO_MAX;

		if (setpriority(PRIO_PROCESS, pid, prio) < 0) {
			switch (errno) {
			case EPERM:
				write_stderr("permission denied\n");
				break;
			default:
				write_stderr("error setting priority\n");
			}

			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}
