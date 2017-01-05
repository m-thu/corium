/*
 * Invocation: rmdir [OPTION/--] DIRECTORY [...]
 *
 * Remove empty DIRECTORY.
 *
 * Options:
 *   -p : remove subdirectories
 *   -- : ignored
 *
 * Return value:
 *   0: success
 *   1: failure
 *
 */

#include "lib.h"

int main(int argc, char *argv[])
{
	if (argc < 2)
		return EXIT_FAILURE;

	int error = EXIT_SUCCESS;
	bool opt_p = false;

	/* parse options */
	if (**++argv == '-') {
		if (argv[0][1] == '-' && argv[0][2] == '\0') {
			++argv;
		} else if (argv[0][1] == 'p' && argv[0][2] == '\0') {
			opt_p = true;
			++argv;
		}
	}

	while (*argv) {
		if (!opt_p) {
			if (rmdir(*argv) < 0) {
				write_stderr("error removing '");
				write_stderr(*argv);
				write(1, "'\n", 2);
				error = EXIT_FAILURE;
			}
		} else {
			char *tmp;
		}
	
		++argv;
	}
	
	return error;
}
