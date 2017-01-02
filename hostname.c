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
		size_t len;

		if (__unlikely(uname(&uts) < 0))
			return EXIT_FAILURE;

		len = strlen(uts.nodename);
		uts.nodename[len] = '\n';
		write(1, uts.nodename, len+1);
	} else if (argc == 2) {
		if (sethostname(argv[1], strlen(argv[1])) < 0) {
			switch (errno) {
			case ENAMETOOLONG:
				write_stderr("name too long\n");
				break;
			case EPERM:
				write_stderr("permission denied\n");
				break;
			default:
				write_stderr("error\n");
				break;
			}

			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}
