/*
 * Invocation: basename NAME [SUFFIX]
 *
 * Print NAME without leading directories and trailing SUFFIX, if specified.
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

	char *s;

	/* remove leading directories */
	while ((s = strchr(argv[1], '/'))) {
		/* keep single '/' */
		if (argv[1][1] == '\0' && argv[1][0] == '/')
			break;

		/* if last character is '/', remove it */
		if (s[1] == '\0') {
			*s = '\0';
			break;
		}

		argv[1] = s + 1;
	}

	/* remove suffix */
	if (argc > 2) {
		size_t len_suffix = strlen(argv[2]);
		size_t len_file   = strlen(argv[1]);

		if (len_suffix <= len_file) {
			if (strncmp(argv[1]+len_file-len_suffix, argv[2],
			            len_suffix) == 0)
				argv[1][len_file-len_suffix] = '\0';
		}
	}

	write_stdout(argv[1]);
	write(1, "\n", 1);

	return EXIT_SUCCESS;
}
