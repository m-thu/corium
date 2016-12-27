#ifndef LIB_X86_64_H
#define LIB_X86_64_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* environment variables */
extern const char **environ;

/* prototypes for functions defined in lib.h */

static size_t strlen(const char *);
static char *strchr(const char *, int);
static int strcmp(const char *, const char *);
static int strncmp(const char *, const char *, size_t);

/* definitions from sys/types.h */

typedef int64_t ssize_t;

/* errno.h */
extern int errno;

/* stdlib.h */

static void __attribute__((unused))
_exit(int status)
{
	asm volatile ("movq $60, %%rax \n\t"
		"syscall"
		:: "D" (status)
		: "memory");
}

static char * __attribute__((unused))
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

static char * __attribute__((unused))
getcwd(char *buf, size_t size)
{
	int ret;

	asm volatile ("movq $79, %%rax \n\t"
		"syscall"
		: "=a" (ret)
		: "D" (buf), "S" (size)
		: "%rcx", "%r11", "cc", "memory");

	if (ret >= 0)
		return buf;

	errno = -ret;
	return NULL;
}

static int __attribute__((unused))
sethostname(const char *name, size_t len)
{
	int ret;

	asm volatile ("movq $170, %%rax \n\t"
		"syscall"
		: "=a" (ret)
		: "D" (name), "S" (len)
		: "%rcx", "%r11", "cc", "memory");

	if (ret < 0) {
		errno = -ret;
		return -1;
	}

	return 0;
}

static ssize_t __attribute__((unused))
write(int fd, const void *buf, size_t count)
{
	ssize_t ret;

	asm volatile ("movq $1, %%rax \n\t"
		"syscall"
		: "=a" (ret)
		: "D" (fd), "S" (buf), "d" (count)
		: "%rcx", "%r11", "cc", "memory");

	if (ret < 0) {
		errno = -ret;
		return -1;
	}

	return ret;
}

/* sys/utsname.h */

#define __NEW_UTS_LEN 64
struct utsname {
	char sysname[__NEW_UTS_LEN + 1];
	char nodename[__NEW_UTS_LEN + 1];
	char release[__NEW_UTS_LEN + 1];
	char version[__NEW_UTS_LEN + 1];
	char machine[__NEW_UTS_LEN + 1];
	char domainname[__NEW_UTS_LEN + 1];
};

static int __attribute__((unused))
uname(struct utsname *buf)
{
	int ret;

	asm volatile ("movq $63, %%rax \n\t"
		"syscall"
		: "=a" (ret)
		: "D" (buf)
		: "%rcx", "%r11", "cc", "memory");

	if (ret < 0) {
		errno = -ret;
		return -1;
	}

	return 0;
}

#endif
