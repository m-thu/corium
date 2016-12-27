/*
 * Invocation: pwd [OPTION]
 *
 * Print current working directory
 *
 * Options:
 *   -L : get working directory from environment
 *
 * TODO:
 *   -P : avoid all symlinks
 *
 * Return value:
 *   0: success
 *   1: failure
 *
 */

#include "lib.h"

#ifndef PATH_MAX
#define PATH_MAX 4096 /* from linux/limits.h */
#endif

int main(int argc, char *argv[])
{
	char wd[PATH_MAX];
	char *wd_env;

	if (argc > 1 && argv[1][0] == '-' && argv[1][1] == 'L') {
		if ((wd_env = getenv("PWD")) != NULL) {
			write_stdout(wd_env);
			write(1, "\n", 1);
		} else {
			exit(EXIT_FAILURE);
		}
	} else {
		if (getcwd(wd, PATH_MAX)) {
			write_stdout(wd);
			write(1, "\n", 1);
		} else {
			exit(EXIT_FAILURE);
		}
	}

	return EXIT_SUCCESS;
}
