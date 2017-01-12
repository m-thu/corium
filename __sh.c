// useless shell, test for execvp, fork, waitpid

#include "lib.h"

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

/* builtin commands */
static void cmd_exit(char **);
static void cmd_cd(char **);

#define CMD(name) { #name, cmd_ ## name }

static struct {
	const char *name;
	void (*function)(char **);
} builtin[] = {
	CMD (exit),
	CMD (cd),
	{NULL, NULL}
};

static bool read_line(char *, size_t);
static void split_line(char *, char **);

int main()
{
	char line[1024];
	char *argv[1024];
	pid_t pid;
	int status;
	char cwd[PATH_MAX];
	char prompt[] = {'$', ' '};
	struct utsname uts;

	if (geteuid() == 0)
		*prompt = '#';
	uname(&uts);

	for (;;) {
		write_stdout(uts.nodename);
		write(1, ":", 1);
		if (getcwd(cwd, sizeof cwd) == NULL)
			*cwd = '\0';
		write_stdout(cwd);
		write(1, prompt, sizeof prompt);

		/* exit on CTRL-D */
		if (!read_line(line, sizeof line-1))
			break;

		/* empty command ? */
		if (*line == '\0')
			continue;

		split_line(line, argv);

		/* builtin command */
		for (size_t i = 0; builtin[i].name; ++i) {
			if (strcmp(builtin[i].name, *argv) == 0) {
				builtin[i].function(argv);
				continue;
			}
		}

		/* external command */
		if ((pid = fork()) > 0) {
			/* parent */
			do {
				if (waitpid(pid, &status, WUNTRACED) < 0) {
					write_stderr("error: waitpid\n");
					_exit(EXIT_FAILURE);
				}
			} while (!WIFEXITED(status) && !WIFSIGNALED(status));
		} else if (pid == 0) {
			/* child */
			if (execvp(*argv, argv) < 0)
				_exit(EXIT_FAILURE);
		} else {
			write_stderr("error: fork\n");
		}
	}

	return EXIT_SUCCESS;
}

/* len: size of buffer excluding zero terminator */
static bool read_line(char *s, size_t len)
{
	int ret;
	char buf;

	while ((ret = read(0, &buf, 1)) > 0) {
		/* buffer full or newline: execute command */
		if (len == 0 || buf == '\n') {
			*s = '\0';
			return true;
		}

		*s++ = buf;
		--len;
	}

	/* EOF: exit shell */
	if (ret == 0)
		return false;

	write_stderr("error: read\n");
	_exit(EXIT_FAILURE);

	/* suppress compiler warning */
	return false;
}

static void split_line(char *s, char *argv[])
{
	enum {
		EAT_SPACE,
		READ_ARG
	} state = EAT_SPACE;

	while (*s) {
		switch (state) {
		case EAT_SPACE:
			if (!isspace((unsigned char)*s)) {
				/* found start of cmd/arg */
				*argv++ = s;
				state = READ_ARG;
			}
			break;

		case READ_ARG:
			if (isspace((unsigned char)*s)) {
				/* found end of cmd/arg,
				   zero terminate string */
				*s = '\0';
				state = EAT_SPACE;
			}
			break;
		}
		++s;
	}

	/* zero terminate array of pointers to cmd/args */
	*argv = NULL;
}

static void cmd_exit(char *argv[])
{
	(void)argv;

	_exit(EXIT_SUCCESS);
}

static void cmd_cd(char *argv[])
{
	if (argv[1] == NULL) {
		char *home = getenv("HOME");

		if (home)
			chdir(home);
	} else {
		if (chdir(argv[1]) < 0) {
			write_stderr(argv[1]);
			switch (errno) {
			case EACCES:
				write_stderr(": Permission denied\n");
				break;
			case ENOTDIR:
				write_stderr(": Not a directory\n");
				break;
			case ENOENT:
				write_stderr(": No such file or directory\n");
				break;
			default:
				write_stderr(": Error\n");
			}
		}
	}
}
