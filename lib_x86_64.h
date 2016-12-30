#ifndef LIB_X86_64_H
#define LIB_X86_64_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdarg.h>

/* environment variables */
extern const char **environ;

/* prototypes for functions defined in lib.h */

static size_t strlen(const char *);
static char *strchr(const char *, int);
static int strcmp(const char *, const char *);
static int strncmp(const char *, const char *, size_t);

/* definitions from sys/types.h */

typedef int64_t  ssize_t;
typedef uint32_t mode_t;

/* errno.h */

extern int errno;

#define EPERM            1
#define ENOENT           2
#define ESRCH            3
#define EINTR            4
#define EIO              5
#define ENXIO            6
#define E2BIG            7
#define ENOEXEC          8
#define EBADF            9
#define ECHILD          10
#define EAGAIN          11
#define ENOMEM          12
#define EACCES          13
#define EFAULT          14
#define ENOTBLK         15
#define EBUSY           16
#define EEXIST          17
#define EXDEV           18
#define ENODEV          19
#define ENOTDIR         20
#define EISDIR          21
#define EINVAL          22
#define ENFILE          23
#define EMFILE          24
#define ENOTTY          25
#define ETXTBSY         26
#define EFBIG           27
#define ENOSPC          28
#define ESPIPE          29
#define EROFS           30
#define EMLINK          31
#define EPIPE           32
#define EDOM            33
#define ERANGE          34
#define EDEADLK         35
#define ENAMETOOLONG    36
#define ENOLCK          37
#define ENOSYS          38
#define ENOTEMPTY       39
#define ELOOP           40
#define EWOULDBLOCK     EAGAIN
#define ENOMSG          42
#define EIDRM           43
#define ECHRNG          44
#define EL2NSYNC        45
#define EL3HLT          46
#define EL3RST          47
#define ELNRNG          48
#define EUNATCH         49
#define ENOCSI          50
#define EL2HLT          51
#define EBADE           52
#define EBADR           53
#define EXFULL          54
#define ENOANO          55
#define EBADRQC         56
#define EBADSLT         57
#define EDEADLOCK       EDEADLK
#define EBFONT          59
#define ENOSTR          60
#define ENODATA         61
#define ETIME           62
#define ENOSR           63
#define ENONET          64
#define ENOPKG          65
#define EREMOTE         66
#define ENOLINK         67
#define EADV            68
#define ESRMNT          69
#define ECOMM           70
#define EPROTO          71
#define EMULTIHOP       72
#define EDOTDOT         73
#define EBADMSG         74
#define EOVERFLOW       75
#define ENOTUNIQ        76
#define EBADFD          77
#define EREMCHG         78
#define ELIBACC         79
#define ELIBBAD         80
#define ELIBSCN         81
#define ELIBMAX         82
#define ELIBEXEC        83
#define EILSEQ          84
#define ERESTART        85
#define ESTRPIPE        86
#define EUSERS          87
#define ENOTSOCK        88
#define EDESTADDRREQ    89
#define EMSGSIZE        90
#define EPROTOTYPE      91
#define ENOPROTOOPT     92
#define EPROTONOSUPPORT 93
#define ESOCKTNOSUPPORT 94
#define EOPNOTSUPP      95
#define EPFNOSUPPORT    96
#define EAFNOSUPPORT    97
#define EADDRINUSE      98
#define EADDRNOTAVAIL   99
#define ENETDOWN        100
#define ENETUNREACH     101
#define ENETRESET       102
#define ECONNABORTED    103
#define ECONNRESET      104
#define ENOBUFS         105
#define EISCONN         106
#define ENOTCONN        107
#define ESHUTDOWN       108
#define ETOOMANYREFS    109
#define ETIMEDOUT       110
#define ECONNREFUSED    111
#define EHOSTDOWN       112
#define EHOSTUNREACH    113
#define EALREADY        114
#define EINPROGRESS     115
#define ESTALE          116
#define EUCLEAN         117
#define ENOTNAM         118
#define ENAVAIL         119
#define EISNAM          120
#define EREMOTEIO       121
#define EDQUOT          122
#define ENOMEDIUM       123
#define EMEDIUMTYPE     124
#define ECANCELED       125
#define ENOKEY          126
#define EKEYEXPIRED     127
#define EKEYREVOKED     128
#define EKEYREJECTED    129
#define EOWNERDEAD      130
#define ENOTRECOVERABLE 131
#define ERFKILL         132
#define EHWPOISON       133

/* fcntl.h */

#define O_RDONLY    00
#define O_WRONLY    01
#define O_RDWR      02
#define O_APPEND    02000
#define O_ASYNC     020000
#define O_CLOEXEC   02000000
#define O_CREAT     0100
#define O_DIRECT    040000
#define O_DIRECTORY 0200000
#define O_EXCL      0200
#define O_LARGEFILE 0100000
#define O_NOATIME   01000000
#define O_NOCTTY    0400
#define O_NOFOLLOW  0400000
#define O_NONBLOCK  04000
#define O_NDELAY    O_NONBLOCK
#define O_PATH      010000000
#define O_SYNC      04010000
#define O_TRUNC     01000

static int __attribute__((unused))
open(const char *pathname, int flags, ...)
{
	int ret;
	mode_t mode = 0;
	va_list ap;

	va_start(ap, flags);
	if (flags & O_CREAT)
		mode = va_arg(ap, mode_t);
	va_end(ap);

	asm volatile ("movq $2, %%rax \n\t"
		"syscall"
		: "=a" (ret)
		: "D" (pathname), "S" (flags), "d" (mode)
		: "%rcx", "%r11", "cc", "memory");

	if (ret >= 0)
		return ret;

	errno = -ret;
	return -1;
}

static inline int __attribute__((unused))
creat(const char *pathname, mode_t mode)
{
	return open(pathname, O_CREAT|O_WRONLY|O_TRUNC, mode);
}

/* linux/vt.h */

#define VT_ACTIVATE   0x5606
#define VT_WAITACTIVE 0x5607

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

/* time.h */

struct timespec {
	long tv_sec;
	long tv_nsec;
};

static int __attribute__((unused))
nanosleep(const struct timespec *req, struct timespec *rem)
{
	int ret;

	asm volatile ("movq $35, %%rax \n\t"
		"syscall"
		: "=a" (ret)
		: "D" (req), "S" (rem)
		: "%rcx", "%r11", "cc", "memory");

	if (ret >= 0)
		return 0;

	errno = -ret;
	return -1;
}

/* unistd.h */

static int __attribute__((unused))
chdir(const char *path)
{
	int ret;

	asm volatile ("movq $80, %%rax \n\t"
		"syscall"
		: "=a" (ret)
		: "D" (path)
		: "%rcx", "%r11", "cc", "memory");

	if (ret >= 0)
		return 0;

	errno = -ret;
	return -1;
}

static int __attribute__((unused))
chroot(const char *path)
{
	int ret;

	asm volatile ("movq $161, %%rax \n\t"
		"syscall"
		: "=a" (ret)
		: "D" (path)
		: "%rcx", "%r11", "cc", "memory");

	if (ret >= 0)
		return 0;

	errno = -ret;
	return -1;
}

static int __attribute__((unused))
close(int fd)
{
	int ret;

	asm volatile ("movq $3, %%rax \n\t"
		"syscall"
		: "=a" (ret)
		: "D" (fd)
		: "%rcx", "%r11", "cc", "memory");

	if (ret >= 0)
		return 0;

	errno = -ret;
	return -1;
}

static int __attribute__((unused))
execve(const char *filename, char *const argv[],
       char *const envp[])
{
	int ret;

	asm volatile ("movq $59, %%rax \n\t"
		"syscall"
		: "=a" (ret)
		: "D" (filename), "S" (argv), "d" (envp)
		: "%rcx", "%r11", "cc", "memory");

	if (ret < 0) {
		errno = -ret;
		return -1;
	}

	/* never reached */
	return 0;
}

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

static ssize_t __attribute__((unused))
read(int fd, void *buf, size_t count)
{
	ssize_t ret;

	asm volatile ("movq $0, %%rax \n\t"
		"syscall"
		: "=a" (ret)
		: "D" (fd), "S" (buf), "d" (count)
		: "%rcx", "%r11", "cc", "memory");

	if (ret >= 0)
		return ret;

	errno = -ret;
	return -1;
}

static int __attribute__((unused))
setdomainname(const char *name, size_t len)
{
	int ret;

	asm volatile ("movq $171, %%rax \n\t"
		"syscall"
		: "=a" (ret)
		: "D" (name), "S" (len)
		: "%rcx", "%r11", "cc", "memory");

	if (ret >= 0)
		return 0;

	errno = -ret;
	return -1;
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

	if (ret >= 0)
		return 0;

	errno = -ret;
	return -1;
}

static unsigned int __attribute__((unused))
sleep(unsigned int seconds)
{
	struct timespec req, rem;

	req.tv_sec = seconds;
	req.tv_nsec = 0;
	nanosleep(&req, &rem);

	return seconds - rem.tv_sec;
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

	if (ret >= 0)
		return ret;

	errno = -ret;
	return -1;
}

/* sys/ioctl.h */

static int __attribute__((unused))
ioctl(int d, int request, ...)
{
	int ret;
	va_list ap;
	size_t arg;

	va_start(ap, request);
	arg = va_arg(ap, size_t);
	va_end(ap);

	asm volatile ("movq $16, %%rax \n\t"
		"syscall"
		: "=a" (ret)
		: "D" (d), "S" (request), "d" (arg)
		: "%rcx", "%r11", "cc", "memory");

	if (ret >= 0)
		return ret;

	errno = -ret;
	return -1;
}

/* sys/stat.h */

#define S_IRWXU 00700
#define S_IRUSR 00400
#define S_IWUSR 00200
#define S_IXUSR 00100

#define S_IRWXG 00070
#define S_IRGRP 00040
#define S_IWGRP 00020
#define S_IXGRP 00010

#define S_IRWXO 00007
#define S_IROTH 00004
#define S_IWOTH 00002
#define S_IXOTH 00001

/* sys/utsname.h */

#define __NEW_UTS_LEN 64
struct utsname {
	char sysname   [__NEW_UTS_LEN + 1];
	char nodename  [__NEW_UTS_LEN + 1];
	char release   [__NEW_UTS_LEN + 1];
	char version   [__NEW_UTS_LEN + 1];
	char machine   [__NEW_UTS_LEN + 1];
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

	if (ret >= 0)
		return 0;

	errno = -ret;
	return -1;
}

#endif
