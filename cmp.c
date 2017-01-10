/*
 * Invocation: cmp [OPTION] FILE1 FILE2
 *
 * Compare two binary files and write the first difference (byte and line
 * number) to stdout. Reads from stdin if FILENAME is '-'.
 *
 * Options:
 *   -l : print every difference as byte number (decimal) and two
 *        byte values (octal)
 *   -s : silent
 *
 * Return value:
 *   0 : files are identical
 *   1 : files are different
 *   2 : error
 *
 */

#include "lib.h"

#define BUFSIZE 4096

#define EXIT_IDENTICAL 0
#define EXIT_DIFFERENT 1
#define EXIT_ERROR     2

static ssize_t read_file(int, uint8_t *, size_t);
static void print_octal(uint8_t);
static void print_decimal(size_t, size_t);
static void warn_file(char *);

static inline ssize_t min(ssize_t a, ssize_t b)
{
	return a < b ? a : b;
}

int main(int argc, char *argv[])
{
	bool opt_s = false,
	     opt_l = false;

	if (*++argv && **argv == '-') {
		if (argv[0][1] == 's' && argv[0][2] == '\0') {
			opt_s = true;
			--argc;
			++argv;
		} else if (argv[0][1] == 'l' && argv[0][2] == '\0') {
			opt_l = true;
			--argc;
			++argv;
		} else if (argv[0][1] == '-' && argv[0][2] == '\0') {
			--argc;
			++argv;
		}
	}

	if (argc < 3)
		return EXIT_ERROR;

	int fd1, fd2;

	if (argv[0][0] == '-' && argv[0][1] == '\0') {
		/* filename '-': read from stdin */
		fd1 = 0;
	} else {
		if ((fd1 = open(argv[0], O_RDONLY)) < 0) {
			warn_file(argv[0]);
			return EXIT_ERROR;
		}
	}

	if (argv[1][0] == '-' && argv[1][1] == '\0') {
		/* filename '-': read from stdin */
		fd2 = 0;
	} else {
		if ((fd2 = open(argv[1], O_RDONLY)) < 0) {
			warn_file(argv[1]);
			return EXIT_ERROR;
		}
	}

	ssize_t len1, len2;
	int retval;
	uint8_t buf1[BUFSIZE], buf2[BUFSIZE];
	size_t byte = 1, line = 1;

	for (;;) {
		if ((len1 = read_file(fd1, buf1, sizeof buf1)) < 0) {
			warn_file(argv[0]);
			retval = EXIT_ERROR;
			break;
		}
		if ((len2 = read_file(fd2, buf2, sizeof buf2)) < 0) {
			warn_file(argv[1]);
			retval = EXIT_ERROR;
			break;
		}

		for (ssize_t i = 0; i < min(len1, len2); ++i, ++byte) {
			if (buf1[i] == '\n')
				++line;

			if (buf1[i] != buf2[i]) {
				if (opt_l) {
					print_decimal(byte, 5);
					write(1, " ", 1);
					print_octal(buf1[i]);
					write(1, " ", 1);
					print_octal(buf2[i]);
					write(1, "\n", 1);
				} else {
					if (!opt_s) {
						write_stdout(argv[0]);
						write(1, " ", 1);
						write_stdout(argv[1]);
						write_stdout(" differ: byte ");
						print_decimal(byte, 0);
						write_stdout(", line ");
						print_decimal(line, 0);
						write(1, "\n", 1);
					}

					retval = EXIT_DIFFERENT;
					goto error;
				}
			}
		}

		if (len1 != len2) {
			if (!opt_s) {
				write_stderr("cmp: EOF on ");
				write_stderr(len1<len2 ? argv[0] : argv[1]);
				write(2, "\n", 1);
			}

			retval = EXIT_DIFFERENT;
			break;
		}

		if (len1 == 0) {
			retval = EXIT_IDENTICAL;
			break;
		}
	}

error:
	if (fd1)
		close(fd1);
	if (fd2)
		close(fd2);

	return retval;
}

static void warn_file(char *s)
{
	write_stderr("error while processing file '");
	write_stderr(s);
	write_stderr("'\n");
}

static ssize_t read_file(int fd, uint8_t *buf, size_t len)
{
	ssize_t retval = 0, r;

	while (len && (r = read(fd, buf, len))) {
		retval += r;
		buf += r;
		len -= (size_t)r;
	}

	if (r < 0)
		return r;

	return retval;
}

/* printf("%3o", byte) */
static void print_octal(uint8_t byte)
{
	int digit1, digit2, digit3;
	char s[3] = {' ', ' ', ' '};

	digit1 = byte % 8;
	byte /= 8;
	digit2 = byte % 8;
	byte /= 8;
	digit3 = byte % 8;

	if (digit3) {
		s[0] = digit3 + '0';
		s[1] = digit2 + '0';
	} else if (digit2) {
		s[1] = digit2 + '0';
	}
	s[2] = digit1 + '0';

	write(1, s, sizeof s);
}

/* printf("%[pad]lu", n) */
static void print_decimal(size_t n, size_t pad)
{
	size_t len = 1, tmp, i = 0;

	tmp = n;
	while ((tmp /= 10UL))
		++len;

	/* allocate memory on stack */
	char s[len<pad ? pad : len];

	if (len < pad)
		memset(s, ' ', sizeof s);

	while (len--) {
		s[sizeof s-1 - i++] = n % 10UL + '0';
		n /= 10UL;
	}

	write(1, s, sizeof s);
}
