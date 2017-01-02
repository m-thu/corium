/*
 * Invocation: dirname NAME
 *
 * Print NAME without trailing filename
 *
 * Return value:
 *   0: success
 *   1: failure
 *
 */

#include "lib.h"

int main(int argc, char *argv[])
{
	if (argc != 2)
		return EXIT_FAILURE;

	char *s = strrchr(argv[1], '/');
	size_t len = 2;

	if (s) {
		/* keep '/' if it is the first character */
		if (s > argv[1]) {
			*s = '\n';
			s[1] = '\0';
			len = strlen(argv[1]) + 1;
		} else {
			s[1] = '\n';
		}
	} else {
		argv[1][0] = '.';
		argv[1][1] = '\n';
	}

	write(1, argv[1], len);

	return EXIT_SUCCESS;
}
