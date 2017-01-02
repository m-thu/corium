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

int main()
{
	struct utsname uts;

	if (uname(&uts) < 0) {
		return EXIT_FAILURE;
	}

	size_t len = strlen(uts.machine);

	uts.machine[len] = '\n';
	write(1, uts.machine, len+1);

	return EXIT_SUCCESS;
}
