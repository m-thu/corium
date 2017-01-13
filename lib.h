#ifndef LIB_H
#define LIB_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifndef NULL
#define NULL (void *)0
#endif

/* branch optimization */
#define __likely(x)   __builtin_expect(!!(x), 1)
#define __unlikely(x) __builtin_expect(!!(x), 0)

#ifdef __x86_64__
#include "lib_x86_64.h"
#endif

#ifdef __dos__
#include "lib_dos.h"
#endif

/* architecture independent */

#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#endif

#ifndef EXIT_FAILURE
#define EXIT_FAILURE 1
#endif

#ifndef __dos__

/* prototypes for functions in this header file */

static size_t __attribute__((unused)) strlen(const char *);
static void * __attribute__((unused)) memcpy(void *, const void *, size_t);

/* ctype.h */

static int __attribute__((unused))
isalpha(int c)
{
	return (c >= 'a' && c <= 'z')
	       || (c >= 'A' && c <= 'Z');
}

static int __attribute__((unused))
isdigit(int c)
{
	return c >= '0' && c <= '9';
}

static int __attribute__((unused))
isprint(int c)
{
	c &= 0x7f;

	return c >= 32 && c < 127;
}

static int __attribute__((unused))
isspace(int c)
{
	return (c == ' ') || (c == '\f') || (c == '\n') || (c == '\r')
	       || (c == '\t') || (c == '\v');
}

static int __attribute__((unused))
isxdigit(int c)
{
	return (c >= '0' && c <= '9')
	       || (c >= 'a' && c <= 'f')
	       || (c >= 'A' && c <= 'F');
}

static int __attribute__((unused))
tolower(int c)
{
	if (c >= 'A' && c <= 'Z')
		c = 'a' + c - 'A';
	return c;
}

/* stdio.h */

static int __attribute__((unused))
putchar(int c)
{
	write(1, (uint8_t *)&c, 1);

	return c;
}

/* stdlib.h */

static int __attribute__((unused))
atoi(const char *nptr)
{
	int64_t ret = 0;
	bool neg = false;

	/* falls through, if *nptr == '\0' */

	/* eat whitespaces */
	while (isspace((unsigned char)*nptr))
		++nptr;

	/* handle single (optional) +/- */
	if (*nptr == '+')
		++nptr;
	if (*nptr == '-') {
		neg = true;
		++nptr;
	}

	/* convert string to integer (base 10) */
	while (isdigit((unsigned char)*nptr))
		ret = ret*10 + (*nptr++ - '0');

	return neg ? -ret : ret;
}

/* removes variable, if '=' is missing */
static int __attribute__((unused))
putenv(char *string)
{
	/* number of variables in environment including zero terminator */
	static size_t count = 0;
	static char **env = NULL;
	bool remove = (strchr(string, '=') == NULL);
	int retval = 0;

	/* nothing to do if variable doesn't exist */
	if (remove && getenv(string) == NULL)
		goto exit;

	/* if variable already exists, change its value */
	for (char **e = (char **)environ; !remove && *e; ++e) {
		char *end = strchr(*e, '=');

		if (strncmp(*e, string, (size_t)(end-*e)) == 0) {
			*e = string;
			goto exit;
		}
	}

	/* if environment hasn't been reallocated before, calculate its size */
	if (count == 0) {
		count = 1; /* zero terminator */
		for (char **e = (char **)environ; *e; ++e, ++count)
			;
	}

	/* reallocate environment */
	size_t new_count = remove ? count - 1 : count + 1;
	char **new_env;

	if ((new_env = mmap(NULL, new_count * sizeof(char *),
		            PROT_READ|PROT_WRITE,
	                    MAP_PRIVATE|MAP_ANONYMOUS, 0, 0)) == (void *)-1) {
		retval = 1;
		errno = ENOMEM;
		goto exit;
	}

	if (remove) {
		/* remove variable */
		size_t offset = 0;
		size_t len = strlen(string);

		for (char **e=(char **)environ; strncmp(*e, string, len);
		     ++e, ++offset)
			;
		memcpy(new_env, environ, offset * sizeof(char *));
		memcpy(new_env + offset, environ + offset + 1,
		       (count-1-(offset+1)) * sizeof(char *));
		new_env[count-2] = NULL;

	} else {
		/* add variable */
		memcpy(new_env, environ, count * sizeof(char *));
		new_env[count-1] = string;
		new_env[count] = NULL;
	}

	/* free old environment */
	if (env) {
		if (munmap(env, count * sizeof(char *)) < 0) {
			retval = 1;
			errno = ENOMEM;
			goto exit;
		}
	}
	env = new_env;
	environ = new_env;
	count = new_count;

exit:
	return retval;
}

/* string.h */

static void * __attribute__((unused))
memcpy(void *dest, const void *src, size_t n)
{
	void *ret = dest;
	const uint8_t *__src  = src;
	uint8_t       *__dest = dest;

	while (n--)
		*__dest++ = *__src++;

	return ret;
}

static void * __attribute__((unused))
memset(void *s, int c, size_t n)
{
	void *retval = s;
	uint8_t *p = s;

	while (n--)
		*p++ = c;

	return retval;
}

static char * __attribute__((unused))
strchr(const char *s, int c)
{
	char ch = c;

	while (*s) {
		if (*s == ch)
			return (char *)s;
		++s;
	}

	if (ch != '\0') {
		return NULL;
	} else {
		return (char *)s;
	}
}

static char * __attribute__((unused))
strrchr(const char *s, int c)
{
	char ch = c;
	size_t len = strlen(s);

	/* include NULL byte in search */
	s += len++;
	while (len--) {
		if (*s == ch)
			return (char *)s;
		--s;
	}

	return NULL;
}

static int __attribute__((unused))
strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2)) {
		++s1;
		++s2;
	}

	return *s1 - *s2;
}

static int __attribute__((unused))
strncmp(const char *s1, const char *s2, size_t n)
{
	const char *last = s1 + n;

	while (*s1 && s1 != last) {
		if (*s1 != *s2)
			return *s1 - *s2;
		++s1;
		++s2;
	}

	return 0;
}

static size_t __attribute__((unused))
strlen(const char *s)
{
	size_t len = 0;

	while (*s++)
		++len;

	return len;
}

/* unistd.h */

#define PATH_MAX 4096

static void __attribute__((unused))
__shell(const char *file, char *const argv[])
{
	char *default_shell = "/bin/sh";

	/* count number of arguments and allocate memory
	   on stack */
	size_t argc = 0;
	char **arg = (char **)argv;

	while (*arg++)
		++argc;

	char *shell_argv[argc + 2];

	shell_argv[0] = default_shell;
	shell_argv[1] = (char *)file;
	shell_argv[2] = NULL;

	/* handle additional arguments */
	if (argc > 1) {
		size_t i;

		for (i = 1; i < argc; ++i)
			shell_argv[1+i] = argv[i];
		shell_argv[i] = NULL;
	}

	execve(shell_argv[0], shell_argv, environ);
}

static int __attribute__((unused))
execvp(const char *file, char *const argv[])
{
	const char *default_path  = "/bin:/usr/bin";

	/* if file contains '/' treat as absolute filename */
	if (strchr(file, '/')) {
		execve(file, argv, environ);

		/* if execve fails, try executing with shell */
		if (errno == ENOEXEC)
			__shell(file, argv);
	} else {
		char cmd[PATH_MAX];
		const char *path = getenv("PATH");
		const char *start, *end;

		/* if environment variable isn't set, use default path */
		if (path == NULL)
			path = default_path;

		/* search path for executable */
		start = path;
		for (;;) {
			if ((end = strchr(start, ':')) == NULL)
				end = strchr(start, '\0');

			/* max. length: strlen(path) + '/' + strlen(file)
			   + '\0' */
			if ((end-start + 1 + strlen(file) + 1) > PATH_MAX) {
				errno = EINVAL;
				return -1;
			}

			memcpy(cmd, start, (size_t)(end-start));
			cmd[end-start] = '/';
			memcpy(cmd + (size_t)(end-start) + 1, file, strlen(file));
			cmd[end-start+1+strlen(file)] = '\0';

			execve(cmd, argv, environ);

			/* if execve fails, try executing with shell */
			if (errno == ENOEXEC) {
				__shell(cmd, argv);

				/* no further search if execution fails */
				return -1;
			}

			/* no more directories in path, search failed */
			if (*end == '\0')
				break;
			start = end + 1;

			/* if permission is denied or file doesn't exist, try next dir in path */
			if (errno == EACCES || errno == ENOENT || errno == ENOTDIR)
				continue;

			/* execution failed */
			break;
		}
	}

	return -1;
}

static int __attribute__((unused))
nice(int inc)
{
	int prio, ret;

	errno = 0;

	if ((ret = getpriority(PRIO_PROCESS, 0)) == -1) {
		if (errno != 0)
			return -1;
	}

	prio = ret + inc;
	if (prio > PRIO_MAX)
		prio = PRIO_MAX;
	if (prio < PRIO_MIN)
		prio = PRIO_MIN;

	if (setpriority(PRIO_PROCESS, 0, prio) < 0) {
		if (errno == EACCES)
			errno = EPERM;
		return -1;
	}

	if ((ret = getpriority(PRIO_PROCESS, 0)) == -1) {
		if (errno != 0)
			return -1;
	}

	return ret;
}

/* misc */

static inline void __attribute__((unused))
write_stdout(const char *s)
{
	write(1, s, strlen(s));
}

static inline void __attribute__((unused))
write_stderr(const char *s)
{
	write(2, s, strlen(s));
}

/* ifndef __dos__ */
#endif

#endif
