/*
 * Invocation: echo [OPTION] [STRING] [...]
 *
 * Print string(s)
 *
 * Options:
 *   -n : no trailing newline
 *   -e : interpret escape sequences (GNU coreutils extension)
 *
 * Valid escape sequences:
 *   \\ : backslash
 *   \a : bell
 *   \b : backspace
 *   \e : escape
 *   \f : form feed
 *   \n : new line
 *   \r : carriage return
 *   \t : horizontal tab
 *   \v : vertical tab
 *
 *   \c : no further output
 *
 *   \0NNN : octal   NOT IMPLEMENTED
 *   \xHH  : hex
 *
 * Return value:
 *   0: success
 *   1: failure
 *
 */

#include "lib.h"

static void handle_escape(char *);

static inline uint8_t hextoint(int c)
{
	return isdigit(c) ?  c - '0' : tolower(c) - 'a' + 10;
}

int main(int argc, char *argv[])
{
	(void)argc;

	bool opt_n = false,
	     opt_e = false;
	bool first = true;

	/* parse options */
	while (*++argv) {
		if ((*argv)[0] != '-')
			break;
		if ((*argv)[1] == '\0')
			break;

		if ((*argv)[1] == 'n' && (*argv)[2] == '\0') {
			opt_n = true;
		} else if ((*argv)[1] == 'e' && (*argv)[2] == '\0') {
			opt_e = true;
		} else {
			break;
		}
	}

	while (*argv) {
		if (first) {
			first = false;
		} else {
			write(1, " ", 1);
		}

		if (opt_e) {
			handle_escape(*argv);
		} else {
			write_stdout(*argv);
		}

		++argv;
	};

	if (!opt_n)
		write(1, "\n", 1);

	return EXIT_SUCCESS;
}

static void handle_escape(char *s)
{
	do {
		if (*s == '\\') {
			switch (*++s) {
			/* end of argument string */
			case '\0':
				write(1, "\\", 1);
				return;
			/* produce no further output */
			case 'c':
				_exit(EXIT_SUCCESS);

			case '\\':
				write(1, "\\", 1);
				break;
			case 'a':
				write(1, "\a", 1);
				break;
			case 'b':
				write(1, "\b", 1);
				break;
			case 'e':
				/* non-ISO-standard escape sequence */
				write(1, "\x1b", 1);
				break;
			case 'f':
				write(1, "\f", 1);
				break;
			case 'n':
				write(1, "\n", 1);
				break;
			case 'r':
				write(1, "\r", 1);
				break;
			case 't':
				write(1, "\t", 1);
				break;
			case 'v':
				write(1, "\v", 1);
				break;
			case 'x':
				if (isxdigit((unsigned char)s[1])) {
					if (isxdigit((unsigned char)s[2])) {
						putchar(hextoint((unsigned char)s[1])<<4
						        |hextoint((unsigned char)s[2]));
						s += 2;
					} else {
						putchar(hextoint((unsigned char)s[1]));
						++s;
					}
					break;
				}
			default:
				write(1, "\\", 1);
				write(1, s, 1);
			}
		} else {
			write(1, s, 1);
		}
	} while (*++s);
}
