/*
 * Invocation: nohup COMMAND [ARG [...]]
 *
 * Execute COMMAND with optional ARGs and ignore SIGHUP.
 * Stdout is redirected to the file "nohup.out" if it is a terminal.
 * If stderr is a terminal, too, it is redirected to stdout.
 * "nohup.out" will be created in the user's home directory if it cannot
 * be created in the current working directory.
 *
 * Return value:
 *   127: failure
 *
 */

#include "lib.h"

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

#define EXIT_ERROR 127

int main(int argc, char *argv[])
{
	if (argc < 2)
		return EXIT_ERROR;

	/* redirect stdout to "nohup.out", if it is a terminal */
	if (isatty(1)) {
		int fd;
		const char *file = "nohup.out";
		char path[PATH_MAX], *home;
		size_t len_home, len_file;

		if ((fd = open(file, O_RDWR|O_CREAT|O_APPEND,
		               S_IRUSR|S_IWUSR)) < 0) {
			if ((home = getenv("HOME")) == NULL
			    || *home == '\0')
				goto exit;

			len_home = strlen(home);
			len_file = strlen(file);
			if ((len_home+1+len_file+1) > sizeof path)
				goto exit;

			memcpy(path, home, len_home);
			path[len_home] = '/';
			memcpy(path+len_home+1, file, len_file+1);

			if ((fd = open(path, O_RDWR|O_CREAT|O_APPEND,
			               S_IRUSR|S_IWUSR)) < 0)
				goto exit;
		}

		if (dup2(fd, 1) < 0)
			goto exit;
	}

	/* redirect stderr to stdout */
	if (isatty(2) && (dup2(1, 2) < 0))
		goto exit;

	/* ignore SIGHUP */
	signal(SIGHUP, SIG_IGN);

	execvp(argv[1], argv+1);

exit:
	return EXIT_ERROR;
}
