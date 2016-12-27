/*
 * Invocation: hostname [HOSTNAME]
 *
 * Display or set hostname
 *
 * Return value:
 *   0: success
 *   1: failure
 *
 */

#include "lib.h"

int main(int argc, char *argv[])
{
	if (argc == 1) {
		struct utsname uts;

		uname(&uts);
		write_stdout(uts.nodename);
		write(1, "\n", 1);
	} else if (argc == 2) {
		if (sethostname(argv[1], strlen(argv[1])) < 0)
			return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
