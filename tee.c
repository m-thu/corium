/*
 * Invocation: tee [OPTION] [FILENAME [...]]
 *
 * Read from stdin and write to stdout and optionally FILENAME(s).
 *
 * Options:
 *   -a : append to output files instead of overwriting them
 *   -i : ignore SIGINT signal
 *   -- : ignored
 *
 * Return value:
 *   0: success
 *   1: failure
 *
 */

#include "lib.h"

/* buffer size for input buffer */
#define BUFSIZE 4096

static void warn_file(char *);

int main(int argc, char *argv[])
{
	int retval = EXIT_SUCCESS;
	bool opt_a = false;

	/* parse options */
	while (*++argv) {
		if (**argv != '-' || argv[0][1] == '\0')
			/* not an option */
			break;

		char *tmp = *argv;

		while (*++*argv) {
			switch (**argv) {
			case 'i':
				signal(SIGINT, SIG_IGN);
				break;
			case 'a':
				opt_a = true;
				break;
			/* treat any further arguments beginning with
			   '-' as filenames, not as options */
			case '-':
				--argc;
				++argv;
				goto bailout;
			/* invalid option, bail out */
			default:
				*argv = tmp;
				goto bailout;
			}
		}

		--argc;
	}

bailout:
	/* useless, but goto isn't allowed to jump to variable definitions */
	--argc;
	/* allocate memory for file descriptors on stack */
	int fd[++argc];
	/* always write to stdout */
	fd[0] = 1;

	/* open output files */
	for (int i = 1; i < argc; ++i) {
		if ((fd[i] = open(argv[i-1], opt_a ? O_WRONLY|O_APPEND|O_CREAT:
		                  O_WRONLY|O_CREAT|O_TRUNC, 0644)) < 0) {
			warn_file(argv[i-1]);
			retval = EXIT_FAILURE;
			fd[i] = 0;
		}
	}

	uint8_t buf[BUFSIZE], *buf_out;
	ssize_t in, out;

	while ((in = read(0, buf, BUFSIZE)) > 0) {
		for (int i = 0; i < argc; ++i) {
			if (fd[i]) {
				ssize_t tmp_in = in;

				buf_out = buf;
				while ((out = write(fd[i], buf_out, tmp_in)) > 0) {
					tmp_in -= out;
					buf_out += out;
				}

				if (__unlikely(out < 0)) {
					warn_file(argv[i-1]);
					retval = EXIT_FAILURE;
				}
			}
		}
	}

	if (in < 0) {
		write_stderr("read error\n");
		retval = EXIT_FAILURE;
	}

	/* close output files */
	for (int i = 1; i < argc; ++i) {
		if (fd[i]) {
			if (close(fd[i]) < 0)
				warn_file(argv[i-1]);
		}
	}

	return retval;
}

static void warn_file(char *s)
{
	write_stderr("error while processing file '");
	write_stderr(s);
	write_stderr("'\n");
}
