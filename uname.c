/*
 * Invocation: uname [OPTION]
 *
 * Print system info to stdout
 *
 * Options:
 *   -a : print everything (in this order)
 *   -s : kernel release
 *   -n : node name
 *   -r : kernel release
 *   -v : kernel version
 *   -m : machine hardware name
 *   -p : processor/unknown
 *
 * No option specified:
 *   same as '-s'
 *
 * Unsupported options from GNU coreutils:
 *   -i : hardware platform/unknown
 *   -o : operating system
 *
 * Return value:
 *   0: success
 *   1: failure
 *
 */

#include "lib.h"

enum {
	OPT_S = 1<<0,
	OPT_N = 1<<1,
	OPT_R = 1<<2,
	OPT_V = 1<<3,
	OPT_M = 1<<4,
	OPT_P = 1<<5
};

static void print_property(char *);

int main(int argc, char *argv[])
{
	struct utsname uts;
	int opt = 0;

	if (uname(&uts) < 0) {
		return EXIT_FAILURE;
	}

	/* No option: -s */
	if (argc == 1)
		opt |= OPT_S;

	/* Parse options */
	for (int i = 1; i < argc; ++i) {
		/* Skip invalid arguments */
		if (argv[i][0] != '-')
			continue;
		/* Skip incomplete options */
		if (argv[i][1] == '\0')
			continue;

		switch (argv[i][1]) {
		case 'a':
			opt |= OPT_S|OPT_N|OPT_R|OPT_V|OPT_M|OPT_P;
			break;

		case 's':
			opt |= OPT_S;
			break;

		case 'n':
			opt |= OPT_N;
			break;

		case 'r':
			opt |= OPT_R;
			break;

		case 'v':
			opt |= OPT_V;
			break;

		case 'm':
			opt |= OPT_M;
			break;

		case 'p':
			opt |= OPT_P;
			break;

		/* Ignore unknown options */
		default:
			break;
		}
	}

	if (opt & OPT_S)
		print_property(uts.sysname);
	if (opt & OPT_N)
		print_property(uts.nodename);
	if (opt & OPT_R)
		print_property(uts.release);
	if (opt & OPT_V)
		print_property(uts.version);
	if (opt & OPT_M)
		print_property(uts.machine);
	if (opt & OPT_P)
		if ((opt&(OPT_S|OPT_N|OPT_R|OPT_V|OPT_M|OPT_P))
		    != (OPT_S|OPT_N|OPT_R|OPT_V|OPT_M|OPT_P))
			print_property("unknown");
	write(1, "\n", 1);

	return EXIT_SUCCESS;
}

static void print_property(char *s)
{
	static bool first = true;

	if (first) {
		first = false;
	} else {
		write(1, " ", 1);
	}

	write_stdout(s);
}
