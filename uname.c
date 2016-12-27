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

static void print_property(char *);

int main(int argc, char *argv[])
{
	struct utsname uts;
	bool opt_a = false,
	     opt_s = false,
	     opt_n = false,
	     opt_r = false,
	     opt_v = false,
	     opt_m = false,
	     opt_p = false;

	if (uname(&uts) < 0) {
		return EXIT_FAILURE;
	}

	/* No option: -s */
	if (argc == 1)
		opt_s = true;

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
			opt_a = true;
			opt_s = true;
			opt_n = true;
			opt_r = true;
			opt_v = true;
			opt_m = true;
			opt_p = true;
			break;

		case 's':
			opt_s = true;
			break;

		case 'n':
			opt_n = true;
			break;

		case 'r':
			opt_r = true;
			break;

		case 'v':
			opt_v = true;
			break;

		case 'm':
			opt_m = true;
			break;

		case 'p':
			opt_p = true;
			break;

		/* Ignore unknown options */
		default:
			break;
		}
	}

	if (opt_s)
		print_property(uts.sysname);
	if (opt_n)
		print_property(uts.nodename);
	if (opt_r)
		print_property(uts.release);
	if (opt_v)
		print_property(uts.version);
	if (opt_m)
		print_property(uts.machine);
	if (opt_p)
		if (!opt_a)
			write_stdout("unknown");
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
