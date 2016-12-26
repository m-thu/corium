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

/* string.h */

static size_t __attribute__((unused))
strlen(const char *s)
{
	size_t len = 0;

	while (*s++)
		++len;

	return len;
}

#endif
