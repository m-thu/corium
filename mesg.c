/*
 * Invocation: mesg [n|y]
 *
 * (Do not) display messages from other users.
 * Print status, if no options specified.
 *
 * Return value:
 *   0: messages are allowed
 *   1: messages are not allowed
 *   2: error
 *
 */

#include "lib.h"

#define EXIT_MSG_ALLOWED     0
#define EXIT_MSG_NOT_ALLOWED 1
#define EXIT_ERROR           2

int main(int argc, char *argv[])
{
	if (!isatty(0)) {
		write_stderr("not a tty\n");
		return EXIT_ERROR;
	}

	struct stat st;

	if (__unlikely(fstat(0, &st) < 0))
		return EXIT_ERROR;

	if (argc == 1) {
		write_stdout(st.st_mode & S_IWGRP ? "is y\n" : "is n\n");
		return st.st_mode & S_IWGRP ? EXIT_SUCCESS : EXIT_FAILURE;
	} else if (argc == 2) {
		if (argv[1][0] == 'y' && argv[1][1] == '\0') {
			if (__likely(fchmod(0, st.st_mode | S_IWGRP) >= 0))
			 	return EXIT_MSG_ALLOWED;
		}

		if (argv[1][0] == 'n' && argv[1][1] == '\0') {
			if (__likely(fchmod(0, st.st_mode & ~S_IWGRP)))
				return EXIT_MSG_NOT_ALLOWED;
		}
	}

	return EXIT_ERROR;
}
