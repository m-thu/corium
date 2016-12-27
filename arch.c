/*
 * Invocation: arch
 *
 * Print machine hardware name (same as uname -m)
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
	(void)argv;

	struct utsname uts;

	if (uname(&uts) < 0) {
		return EXIT_FAILURE;
	}

	write_stdout(uts.machine);
	write(1, "\n", 1);

	return EXIT_SUCCESS;
}
