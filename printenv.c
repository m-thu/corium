/*
 * Invocation: printenv [VARIABLE] [...]
 *
 * Print environment variable(s) (all if none are specified)
 *
 * Return value:
 *   0: success
 *   1: failure
 *
 */

#include "lib.h"

int main(int argc, char *argv[], char *envp[])
{
	if (argc == 1 ) {
		while (*envp) {
			write_stdout(*envp++);
			write(1, "\n", 1);
		}
	} else {
		while (*++argv) {
			char *var = getenv(*argv);

			if (var) {
				write_stdout(var);
				write(1, "\n", 1);
			}
		}
	}

	return EXIT_SUCCESS;
}
