/*
 * Invocation: sleep NUMBER[SUFFIX] [NUMBER[SUFFIX]] [...]
 *
 * Sleep for number of seconds (sum of arguments)
 *
 * Valid values for SUFFIX:
 *   s : seconds
 *   m : minutes
 *   h : hours
 *   d : days
 *
 * Return value:
 *   0: success
 *   1: failure
 *
 */

#include "lib.h"

int main(int argc, char *argv[])
{
	unsigned int sec = 0;

	if (argc == 1) {
		return EXIT_FAILURE;
	}

	for (int i = 1; i < argc; ++i) {
		unsigned int tmp_sec = 0,
			     mul = 1,
			     weight = 1,
			     len;

		if (!isdigit((unsigned char)argv[i][0]))
			return EXIT_FAILURE;

		len = strlen(argv[i]);

		if (isalpha((unsigned char)argv[i][len-1])) {
			switch (argv[i][len-1]) {
			case 's':
				mul = 1;
				break;
			case 'm':
				mul = 60;
				break;
			case 'h':
				mul = 60*60;
				break;
			case 'd':
				mul = 24*60*60;
				break;
			default:
				return EXIT_FAILURE;

			}
			--len;
		}

		do {
			if (!isdigit((unsigned char)argv[i][len-1]))
				return EXIT_FAILURE;

			tmp_sec += weight * (unsigned int)(argv[i][len-1]-'0');
			weight *= 10;
		} while (--len);

		sec += tmp_sec * mul;
	}

	sleep(sec);

	return EXIT_SUCCESS;
}
