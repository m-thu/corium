#ifndef LIB_H
#define LIB_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifndef NULL
#define NULL (void *)0
#endif

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

/* string.h */

char * __attribute__((unused))
strchr(const char *s, int c)
{
	char ch = c;

	while (*s) {
		if (*s == ch)
			return (char *)s;
		++s;
	}

	if (c != '\0') {
		return NULL;
	} else {
		return (char *)s;
	}
}

int __attribute__((unused))
strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2)) {
		++s1;
		++s2;
	}

	return *s1 - *s2;
}

int __attribute__((unused))
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
