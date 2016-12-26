#ifndef LIB_X86_64_H
#define LIB_X86_64_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* environment variables */
extern const char **environ;

/* prototypes for functions defined in lib.h */

static size_t strlen(const char *);
char *strchr(const char *, int);
int strcmp(const char *, const char *);
int strncmp(const char *, const char *, size_t);

/* definitions from <sys/types.h> */

typedef int64_t ssize_t;

/* stdlib.h */

static void __attribute__((unused))
exit(int status)
{
	asm volatile ("movq $60, %%rax \n\t"
		"syscall"
		:: "D" (status)
		: "memory");
}

char * __attribute__((unused))
getenv(const char *name)
{
	const char **env = environ;

	if (env == NULL)
		return NULL;

	while (*env) {
		char *equ = strchr(*env, '=');

		if (equ == NULL)
			continue;
		if (strncmp(*env, name, equ-*env) == 0)
			return equ + 1;
		++env;
	}

	return NULL;
}

/* unistd.h */

static ssize_t __attribute__((unused))
write(int fd, const void *buf, size_t count)
{
	ssize_t ret;

	asm volatile ("movq $1, %%rax \n\t"
		"syscall"
		: "=a" (ret)
		: "D" (fd), "S" (buf), "d" (count)
		: "%rcx", "%r11", "cc", "memory");
	return ret;
}

#endif
