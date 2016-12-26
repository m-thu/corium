/*
 * Invocation: printenv [OPTION] [VARIABLE] [...]
 *
 * Print environment variable(s) (all if none are specified)
 *
 * Options:
 *   -0 : output '\0' instead of '\n'
 *
 * Return value:
 *   0: success
 *   1: failure
 *
 */

#include "lib.h"

int main(int argc, char *argv[], char *envp[])
{
	bool opt_0 = false;

	if (argc > 1) {
		if (argv[1][0] == '-' && argv[1][1] == '0'
		    && argv[1][2] == '\0')
			opt_0 = true;
	}

	if (argc == 1 || (argc == 2 && opt_0)) {
		while (*envp) {
			write_stdout(*envp++);
			putchar((unsigned char)(opt_0?'\0':'\n'));
		}
	} else {
		for (int i = opt_0?2:1; i < argc; ++i) {
			char *var = getenv(argv[i]);

			if (var) {
				write_stdout(var);
				putchar((unsigned char)(opt_0?'\0':'\n'));
			}
		}
	}

	return EXIT_SUCCESS;
}
