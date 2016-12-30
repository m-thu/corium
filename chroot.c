/*
 * Invocation: chroot NEWROOT [COMMAND [ARG] ...]
 *
 * Run command with root dir set to NEWROOT, runs '/bin/sh -i'
 * if no COMMAND is specified
 *
 * Return value:
 *   0: success
 *   1: failure
 *
 */

#include "lib.h"

static void die(void);

int main(int argc, char *argv[])
{
	char *shell[] = {"/bin/sh", "-i", NULL};

	if (argc == 1)
		return EXIT_FAILURE;

	if (chroot(argv[1]) || chdir("/"))
		die();

	if (argc > 2) {
		execve(argv[2], argv+2, NULL);
	} else {
		execve("/bin/sh", shell, NULL);
	}

	die();
	/* suppress compiler warning */
	return EXIT_FAILURE;
}

static void  die(void)
{
	switch (errno) {
	case EACCES:
		write_stderr("search permission denied");
		break;
	case EFAULT:
		write_stderr("invalid path pointer");
		break;
	case EIO:
		write_stderr("I/O error");
		break;
	case ELOOP:
		write_stderr("too many symlinks");
		break;
	case ENAMETOOLONG:
		write_stderr("path too long");
		break;
	case ENOENT:
		write_stderr("file doesn't exist");
		break;
	case ENOMEM:
		write_stderr("no mem");
		break;
	case ENOTDIR:
		write_stderr("path isn't a directory");
		break;
	case EPERM:
		write_stderr("permission denied");
		break;
	default:
		write_stderr("error");
		break;
	}

	write(2, "\n", 1);
	_exit(EXIT_FAILURE);
}
