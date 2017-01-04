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

/* prototypes for functions in this header file */

static size_t __attribute__((unused)) strlen(const char *);

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
	c &= 0x1f;

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

static int __attribute__((unused))
execvp(const char *file, char *const argv[])
{
	const char *default_path = "/bin:/usr/bin";

	/* if file contains '/' treat as absolute filename */
	if (strchr(file, '/')) {
		execve(file, argv, environ);

		/* if execve fails, try executing with shell */
		if (errno == ENOEXEC) {
			char * const shell[] = {"/bin/sh", "/bin/sh", (char *)file, NULL};
			execve(shell[0], shell, environ);
		}
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
				char * const shell[] = {"/bin/sh", "/bin/sh", (char *)cmd, NULL};
				execve(shell[0], shell, environ);

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

#endif
