/*
 * Invocation: reboot
 *
 * Fore reboot.
 *
 * Return value:
 *   0: success
 *   1: failure
 *
 */

#include "lib.h"

#define LINUX_REBOOT_MAGIC1      0xfee1dead
#define LINUX_REBOOT_MAGIC2      672274793
#define LINUX_REBOOT_CMD_RESTART 0x1234567

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;

	sync();

	if (reboot(LINUX_REBOOT_MAGIC1, LINUX_REBOOT_MAGIC2,
	           LINUX_REBOOT_CMD_RESTART, NULL) < 0) {
		switch (errno) {
		case EINVAL:
			write_stderr("invalid magic or cmd\n");
			break;
		case EPERM:
			write_stderr("permission denied\n");
			break;
		}

		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
