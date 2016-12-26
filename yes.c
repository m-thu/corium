/*
 * Invocation: yes [STRING] [...]
 *
 * Repeatedly prints 'y' or argument string(s) separated by whitespaces
 * on new lines to stdout
 *
 * Return value: always 0
 *
 */

#include "lib.h"

int main(int argc, char *argv[])
{
	char *yes[] = {NULL, "y", NULL};

	if (argc == 1) {
		argc = 2;
		argv = yes;
	}

	for (;;) {
		for (int i = 1; i < argc; ++i) {
			write_stdout(argv[i]);
			if (i < argc-1)
				write(1, " ", 1);
		}
		write(1, "\n", 1);
	}

	return EXIT_SUCCESS;
}
