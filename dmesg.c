/*
 * Invocation: dmesg [OPTION]
 *
 * Print kernel message buffer.
 *
 * Options:
 *   -C : clear ring buffer
 *   -c : print buffer, then clear it
 *   -n : set level at which messages are printed to the console
 *   -r : print raw buffer
 *
 * Valid log levels:
 *   1: alert
 *   2: crit
 *   3: err
 *   4: warn
 *   5: notice
 *   6: info
 *   7: debug
 *   8: ??
 *
 * Return value:
 *   0: success
 *   1: failure
 *
 */

#include "lib.h"

/* buffer size used to query kernel ring buffer */
//#define BUFSIZE 16392
#define BUFSIZE 32*1024

static void clear_buffer(void);

int main(int argc, char *argv[])
{
	/* set log level */
	if (argc == 3 && argv[1][0] == '-' && argv[1][1] == 'n'
	              && argv[1][2] == '\0') {
		if (__syslog(__SYSLOG_ACTION_CONSOLE_LEVEL, NULL,
		             atoi(argv[2])) < 0) {
			switch (errno) {
			case EPERM:
				write_stderr("permission denied\n");
				break;
			case EINVAL:
				write_stderr("invalid log level\n");
				break;
			}

			return EXIT_FAILURE;
		} else {
			return EXIT_SUCCESS;
		}
	}

	bool clear_after_read = false,
	     print_raw        = false;

	if (argc == 2 && argv[1][0] == '-' && argv[1][1] != '\0'
	              && argv[1][2] == '\0') {
		switch (argv[1][1]) {
		/* clear ring buffer and exit */
		case 'C':
			clear_buffer();
			return EXIT_SUCCESS;

		/* print and clear */
		case 'c':
			clear_after_read = true;
			break;

		/* print raw */
		case 'r':
			print_raw = true;
			break;

		/* unknown option */
		default:
			return EXIT_FAILURE;
		}
	}

	/* print buffer */
	char buf[BUFSIZE+1], *out = buf, *tmp;
	int len;

	len = __syslog(__SYSLOG_ACTION_READ_ALL, buf, BUFSIZE);

	if (print_raw) {
		int ret;

		/* print raw buffer */
		while ((ret = write(1, out, len)) > 0) {
			out += ret;
			len -= ret;
		}
	} else {
		/* zero terminate buffer, so we can safely use strchr */
		buf[len] = '\0';

		/* strip loglevel "<n>", skip incomplete lines */
		while (len) {
			/* find start of line */
			if (*out != '<') {
				++out;
				--len;
				continue;
			}

			/* find matching '>' */
			if ((tmp = strchr(out, '>')) == NULL) {
				/* end of buffer with incomplete line reached,
				   exit */
				break;
			}

			/* exit if '>' is last character in buffer */
			if ((len -= ++tmp - out) <= 0)
				break;
			out = tmp;

			/* find end of line */
			if ((tmp = strchr(out, '\n')) == NULL) {
				/* end of buffer with incomplete line reached,
				   exit */
				break;
			}
			len -= ++tmp - out;

			write(1, out, tmp - out);
			out = tmp;
		}
	}

	if (clear_after_read)
		clear_buffer();

	return EXIT_SUCCESS;
}

static void clear_buffer(void)
{
	if (__syslog(__SYSLOG_ACTION_CLEAR, NULL, 0) < 0) {
		if (errno == EPERM)
			write_stderr("permission denied\n");

		_exit(EXIT_FAILURE);
	}
}
