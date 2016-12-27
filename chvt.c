/*
 * Invocation: chvt N
 *
 * Change the foreground terminal to /dev/ttyN
 *
 * Return value:
 *   0: success
 *   1: failure
 *
 */

#include "lib.h"

int main(int argc, char *argv[])
{
	if (argc != 2)
		return EXIT_FAILURE;

	int n = atoi(argv[1]);
#if 0
	int fd = open("/dev/tty0", O_RDONLY);

	if (fd < 0) {
		if ((fd = open("/dev/console", O_RDONLY)) < 0)
			return EXIT_FAILURE;
	}
#endif

	if (ioctl(0, VT_ACTIVATE, n) < 0)
		return EXIT_FAILURE;
	if (ioctl(0, VT_WAITACTIVE, n) < 0)
		return EXIT_FAILURE;

#if 0
	if (close(fd) < 0)
		return EXIT_FAILURE;
#endif

	return EXIT_SUCCESS;
}
