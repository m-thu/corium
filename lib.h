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
	int ret = 0,
	    weight = 1;
	size_t off = 0;

	/* falls through, if *nptr == '\0' */

	/* eat whitespaces */
	while (isspace(*nptr))
		++nptr;

	/* handle single (optional) +/- */
	if (*nptr == '+')
		++nptr;
	if (*nptr == '-') {
		weight = -1;
		++nptr;
	}

	/* find end of integer */
	while (isdigit(*nptr)) {
		++off;
		++nptr;
	}

	/* convert string to integer (base 10) */
	while (off--) {
	       ret += weight * (*--nptr-'0');
	       weight *= 10;
	}

	return ret;
}

/* string.h */

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

static int __attribute__((unused))
nice(int inc)
{
	pid_t pid = getpid();

	if (setpriority(PRIO_PROCESS, pid, inc) < 0) {
		if (errno = EACCES)
			errno = EPERM;
		return -1;
	}

	int ret;

	errno = 0;

	if ((ret = getpriority(PRIO_PROCESS, pid)) == -1) {
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
