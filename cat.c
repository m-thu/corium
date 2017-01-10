/*
 * Invocation: cat [OPTION] [FILENAME ...]
 *
 * Print FILENAME to stdout, reads from stdin when FILENAME is '-'
 * or FILENAME is missing.
 *
 * Options:
 *   -e : print '$' after each newline (implies -v)
 *   -t : print '\t' as '^I' (implies -v)
 *   -u : ignored (unbuffered output)
 *   -v : substitute non-printable characters
 *   -- : ignored
 *
 * Return value:
 *   0: success
 *   1: failure
 *
 */

#include "lib.h"

/* buffer size for cat without options, assumes page size of 4096 */
#define BUFSIZE_RAW 16*4096

/* buffer size for cat with options '-v', '-t', '-e' */
#define BUFSIZE 1024

static int raw_cat(char **);
static int cat(char **, bool, bool);
static void warn_file(char *);

int main(int argc, char *argv[])
{
	(void)argc;

	bool opt_e = false,
	     opt_t = false,
	     opt_v = false;

	/* parse options */
	while (*++argv) {
		if (**argv != '-' || argv[0][1] == '\0')
			/* not an option */
			break;

		char *tmp = *argv;

		while (*++*argv) {
			switch (**argv) {
			case 'e':
				opt_e = true;
				opt_v = true;
				break;
			case 't':
				opt_t = true;
				opt_v = true;
				break;
			case 'u':
				break;
			case 'v':
				opt_v = true;
				break;
			/* treat any further arguments beginning with
			   '-' as filenames, not as options */
			case '-':
				++argv;
				goto bailout;
			/* invalid option, bail out */
			default:
				*argv = tmp;
				goto bailout;
			}
		}
	}

	/* read from stdin, if no files specified */
	if (*argv == NULL) {
		**--argv = '-';
		argv[0][1] = '\0';
	}

bailout:
	if (!opt_v) {
		return raw_cat(argv);
	} else {
		return cat(argv, opt_t, opt_e);
	}
}

static int raw_cat(char *argv[])
{
	int retval = EXIT_SUCCESS;
	uint8_t *buf_in, *buf_out;

	if ((buf_in = mmap(NULL, BUFSIZE_RAW, PROT_READ|PROT_WRITE,
	                   MAP_PRIVATE|MAP_ANONYMOUS, 0, 0)) == (void *)-1) {
		write_stderr("error: mmap\n");
		_exit(EXIT_FAILURE);
	}

	while (*argv) {
		int fd;
		ssize_t in, out;

		if (**argv == '-' && argv[0][1] == '\0') {
			/* filename '-': stdin */
			fd = 0;
		} else {
			if ((fd = open(*argv, O_RDONLY)) < 0) {
				warn_file(*argv++);
				retval = EXIT_FAILURE;
				continue;
			}
		}

		while ((in = read(fd, buf_in, BUFSIZE_RAW)) > 0) {
			buf_out = buf_in;
			while ((out = write(1, buf_out, in)) > 0) {
				in -= out;
				buf_out += out;
			}

			if (__unlikely(out < 0)) {
				warn_file(*argv);
				retval = EXIT_FAILURE;
				break;
			}
		}

		if (in < 0) {
			warn_file(*argv);
			retval = EXIT_FAILURE;
		}

		if (fd) {
			if (close(fd) < 0) {
				retval = EXIT_FAILURE;
				warn_file(*argv);
			}
		}

		++argv;
	}
	
	munmap(buf_in, BUFSIZE_RAW);

	return retval;
}

static int cat(char *argv[], bool opt_t, bool opt_e)
{
	int retval = EXIT_SUCCESS;
	uint8_t buf_in[BUFSIZE], buf_out[BUFSIZE*4];

	while (*argv) {
		int fd;
		ssize_t in, out;

		if (**argv == '-' && argv[0][1] == '\0') {
			/* filename '-': stdin */
			fd = 0;
		} else {
			if ((fd = open(*argv, O_RDONLY)) < 0) {
				warn_file(*argv++);
				retval = EXIT_FAILURE;
				continue;
			}
		}

		while ((in = read(fd, buf_in, sizeof buf_in)) > 0) {
			ssize_t len = 0;

			for (ssize_t i = 0; i < in; ++i) {
				int ch = buf_in[i];
				bool meta = false;

				/* meta characters */
				if (ch & 0x80) {
					buf_out[len++] = 'M';
					buf_out[len++] = '-';
					ch &= 0x7f;
					meta = true;
				}

				/* control characters */
				if (ch >= 0 && ch <= 31) {
					switch (ch) {
					/* HT */
					case 9:
						if (opt_t || meta) {
							buf_out[len++] = '^';
							buf_out[len++] = 'I';
						} else {
							buf_out[len++] = '\t';
						}
						break;
					/* LF */
					case 10:
						if (opt_e) {
							buf_out[len++] = '$';
							buf_out[len++] = '\n';
						} else if (meta) {
							buf_out[len++] = '^';
							buf_out[len++] = 'J';
						} else {
							buf_out[len++] = '\n';
						}
						break;
					default:
						buf_out[len++] = '^';
						buf_out[len++] = ch-1 + 65;/*'@'*/
					}
				/* printable characters */
				} else if (ch > 31 && ch < 127) {
					buf_out[len++] = ch;
				/* DEL */
				} else if (ch == 127) {
					buf_out[len++] = '^';
					buf_out[len++] = '?';
				}
			}

			uint8_t *buf = buf_out;
			while ((out = write(1, buf_out, len)) > 0) {
				len -= out;
				buf += out;
			}

			if (__unlikely(out < 0)) {
				warn_file(*argv);
				retval = EXIT_FAILURE;
				break;
			}
		}

		if (in < 0) {
			warn_file(*argv);
			retval = EXIT_FAILURE;
		}

		if (fd) {
			if (close(fd) < 0) {
				retval = EXIT_FAILURE;
				warn_file(*argv);
			}
		}

		++argv;
	}

	return retval;
}

static void warn_file(char *s)
{
	write_stderr("error while processing file '");
	write_stderr(s);
	write_stderr("'\n");
}
