/*
 * Invocation: domainname [DOMAINNAME]
 *
 * Display or set domainname
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
		write_stdout(uts.domainname);
		write(1, "\n", 1);
	} else if (argc == 2) {
		if (setdomainname(argv[1], strlen(argv[1])) < 0) {
			switch (errno) {
			case EINVAL:
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
