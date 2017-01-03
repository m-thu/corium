#ifndef LIB_X86_64_H
#define LIB_X86_64_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdarg.h>

/* syscalls */

#include "syscalls_x86_64.h"

/* syscall numbers are defined as macros,
   so the argument "nr" to SYSCALLxx has to be
   expanded before stringification */

#define SYSCALL0(nr, retval) \
	__SYSCALL0(nr, retval)
#define __SYSCALL0(nr, retval) \
asm volatile ("movq $"#nr", %%rax \n\t" \
	"syscall" \
	:  "=a" ((retval)) \
	:: "%rcx", "%r11", "cc", "memory");

#define SYSCALL1(nr, param1, retval) \
	__SYSCALL1(nr, param1, retval)
#define __SYSCALL1(nr, param1, retval) \
asm volatile ("movq $"#nr", %%rax \n\t" \
	"syscall" \
	: "=a" ((retval)) \
	: "D" ((param1)) \
	: "%rcx", "%r11", "cc", "memory");

#define SYSCALL2(nr, param1, param2, retval) \
	__SYSCALL2(nr, param1, param2, retval)
#define __SYSCALL2(nr, param1, param2, retval) \
asm volatile ("movq $"#nr", %%rax \n\t" \
	"syscall" \
	: "=a" ((retval)) \
	: "D" ((param1)), "S" ((param2)) \
	: "%rcx", "%r11", "cc", "memory");

#define SYSCALL3(nr, param1, param2, param3, retval) \
	__SYSCALL3(nr, param1, param2, param3, retval)
#define __SYSCALL3(nr, param1, param2, param3, retval) \
asm volatile ("movq $"#nr", %%rax \n\t" \
	"syscall" \
	: "=a" ((retval)) \
	: "D" ((param1)), "S" ((param2)), "d" ((param3)) \
	: "%rcx", "%r11", "cc", "memory");

#define SYSCALL4(nr, param1, param2, param3, param4, retval) \
	__SYSCALL4(nr, param1, param2, param3, param4, retval)
#define __SYSCALL4(nr, param1, param2, param3, param4, retval) \
do { \
	register uint64_t p4 asm("r10") = (uint64_t)(param4); \
	asm volatile ("movq $"#nr", %%rax \n\t" \
		"syscall" \
		: "=a" ((ret)) \
		: "D" ((param1)), "S" ((param2)), "d" ((param3)), "r" ((p4)) \
		: "%rcx", "%r11", "cc", "memory"); \
} while (0);

/* environment variables (defined in start_x86_64.s) */

extern const char **environ;

/* prototypes for functions defined in lib.h */

static size_t strlen(const char *);
static char *strchr(const char *, int);
static int strcmp(const char *, const char *);
static int strncmp(const char *, const char *, size_t);

/* prototypes for this header file */

static int __attribute__((unused)) ioctl(int, int, ...);

/* typedefs from sys/types.h */

typedef uint64_t      dev_t;
typedef unsigned long ino_t;
typedef uint32_t      mode_t;
typedef unsigned long nlink_t;
typedef uint32_t      uid_t;
typedef uint32_t      gid_t;
typedef signed long   off_t;
typedef signed long   blksize_t;
typedef signed long   blkcnt_t;
typedef signed long   time_t;
typedef int64_t       ssize_t;
typedef int32_t       pid_t;

/* typedefs from termios.h */

typedef unsigned char cc_t;
typedef unsigned int  speed_t;
typedef unsigned int  tcflag_t;

/* termios.h */

struct termios {
	tcflag_t c_iflag;
	tcflag_t c_oflag;
	tcflag_t c_cflag;
	tcflag_t c_lflag;
	cc_t     c_line;
	cc_t     c_cc[32];
	speed_t  c_ispeed;
	speed_t  c_ospeed;
};

/* errno.h (defined in start_x86_64.s) */

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

	SYSCALL3(__NR_open, pathname, flags, mode, ret)

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

#define SPLICE_F_MOVE     (1)
#define SPLICE_F_NONBLOCK (2)
#define SPLICE_F_MORE     (4)
#define SPLICE_F_GIFT     (8)

static ssize_t __attribute__((unused))
tee(int fd_in, int fd_out, size_t len, unsigned int flags)
{
	ssize_t ret;

	SYSCALL4(__NR_tee, fd_in, fd_out, len, flags, ret)

	if (ret >= 0)
		return ret;

	errno = -ret;
	return -1;
}

/* linux/reboot.h */

static int __attribute__((unused))
reboot(int magic, int magic2, int cmd, void *arg)
{
	int ret;

	SYSCALL4(__NR_reboot, magic, magic2, cmd, arg, ret)

	if (ret >= 0)
		return 0;

	errno = -ret;
	return -1;
}

/* linux/stat.h */

#define S_IFMT      00170000
#define S_IFSOCK    0140000
#define S_IFLNK     0120000
#define S_IFREG     0100000
#define S_IFBLK     0060000
#define S_IFDIR     0040000
#define S_IFCHR     0020000
#define S_IFIFO     0010000
#define S_ISUID     0004000
#define S_ISGID     0002000
#define S_ISVTX     0001000
#define S_ISLNK(m)  (((m) & S_IFMT) == S_IFLNK)
#define S_ISREG(m)  (((m) & S_IFMT) == S_IFREG)
#define S_ISDIR(m)  (((m) & S_IFMT) == S_IFDIR)
#define S_ISCHR(m)  (((m) & S_IFMT) == S_IFCHR)
#define S_ISBLK(m)  (((m) & S_IFMT) == S_IFBLK)
#define S_ISFIFO(m) (((m) & S_IFMT) == S_IFIFO)
#define S_ISSOCK(m) (((m) & S_IFMT) == S_IFSOCK)
#define S_IRWXU     00700
#define S_IRUSR     00400
#define S_IWUSR     00200
#define S_IXUSR     00100
#define S_IRWXG     00070
#define S_IRGRP     00040
#define S_IWGRP     00020
#define S_IXGRP     00010
#define S_IRWXO     00007
#define S_IROTH     00004
#define S_IWOTH     00002
#define S_IXOTH     00001

/* linux/vt.h */

#define VT_ACTIVATE   (0x5606)
#define VT_WAITACTIVE (0x5607)

/* stdio.h */

static int __attribute__((unused))
rename(const char *oldpath, const char *newpath)
{
	int ret;

	SYSCALL2(__NR_rename, oldpath, newpath, ret)

	if (ret >= 0)
		return 0;

	errno = -ret;
	return -1;
}

/* stdlib.h */

static void __attribute__((unused))
_exit(int status)
{
	int ret;

	SYSCALL1(__NR_exit, status, ret)
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

	SYSCALL2(__NR_nanosleep, req, rem, ret)

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

	SYSCALL1(__NR_chdir, path, ret)

	if (ret >= 0)
		return 0;

	errno = -ret;
	return -1;
}

static int __attribute__((unused))
chown(const char *path, uid_t owner, gid_t group)
{
	int ret;

	SYSCALL3(__NR_chown, path, owner, group, ret)

	if (ret >= 0)
		return 0;

	errno = -ret;
	return -1;
}

static int __attribute__((unused))
fchdir(int fd)
{
	int ret;

	SYSCALL1(__NR_fchdir, fd, ret)

	if (ret >= 0)
		return 0;

	errno = -ret;
	return -1;
}

static int __attribute__((unused))
fchown(int fd, uid_t owner, gid_t group)
{
	int ret;

	SYSCALL3(__NR_fchown, fd, owner, group, ret)

	if (ret >= 0)
		return 0;

	errno = -ret;
	return -1;
}

static int __attribute__((unused))
chroot(const char *path)
{
	int ret;

	SYSCALL1(__NR_chroot, path, ret)

	if (ret >= 0)
		return 0;

	errno = -ret;
	return -1;
}

static int __attribute__((unused))
close(int fd)
{
	int ret;

	SYSCALL1(__NR_close, fd, ret)

	if (ret >= 0)
		return 0;

	errno = -ret;
	return -1;
}

static int __attribute__((unused))
dup(int oldfd)
{
	int ret;

	SYSCALL1(__NR_dup, oldfd, ret)

	if (ret >= 0)
		return ret;

	errno = -ret;
	return -1;
}

static int __attribute__((unused))
dup2(int oldfd, int newfd)
{
	int ret;

	SYSCALL2(__NR_dup2, oldfd, newfd, ret)

	if (ret >= 0)
		return ret;

	errno = -ret;
	return -1;
}

static int __attribute__((unused))
execve(const char *filename, char *const argv[],
       char *const envp[])
{
	int ret;

	SYSCALL3(__NR_execve, filename, argv, envp, ret)

	if (ret < 0) {
		errno = -ret;
		return -1;
	}

	/* never reached */
	return 0;
}

static pid_t __attribute__((unused))
fork(void)
{
	pid_t ret;

	SYSCALL0(__NR_fork, ret)

	if (ret >= 0)
		return ret;

	errno = -ret;
	return -1;
}

struct stat {
	unsigned long st_dev;
	unsigned long st_ino;
	unsigned long st_nlink;
	unsigned int  st_mode;
	unsigned int  st_uid;
	unsigned int  st_gid;
	unsigned int  __pad0;
	unsigned long st_rdev;
	long          st_size;
	long          st_blksize;
	long          st_blocks;
	unsigned long st_atime;
	unsigned long st_atime_nsec;
	unsigned long st_mtime;
	unsigned long st_mtime_nsec;
	unsigned long st_ctime;
	unsigned long st_ctime_nsec;
	long          __unused[3];
};

static int __attribute__((unused))
fstat(int fd, struct stat *buf)
{
	int ret;

	SYSCALL2(__NR_fstat, fd, buf, ret)

	if (ret >= 0)
		return 0;

	errno = -ret;
	return -1;
}

static char * __attribute__((unused))
getcwd(char *buf, size_t size)
{
	int ret;

	SYSCALL2(__NR_getcwd, buf, size, ret)

	if (ret >= 0)
		return buf;

	errno = -ret;
	return NULL;
}

static gid_t __attribute__((unused))
getegid(void)
{
	gid_t ret;

	SYSCALL0(__NR_getegid, ret)

	return ret;
}

static uid_t __attribute__((unused))
geteuid(void)
{
	uid_t ret;

	SYSCALL0(__NR_geteuid, ret)

	return ret;
}

static gid_t __attribute__((unused))
getgid(void)
{
	gid_t ret;

	SYSCALL0(__NR_getgid, ret)

	return ret;
}

static pid_t __attribute__((unused))
getpid(void)
{
	pid_t ret;

	SYSCALL0(__NR_getpid, ret);

	return ret;
}

static pid_t __attribute__((unused))
getppid(void)
{
	pid_t ret;

	SYSCALL0(__NR_getppid, ret)

	return ret;
}

static int __attribute__((unused))
getresgid(gid_t *rgid, gid_t *egid, gid_t *sgid)
{
	int ret;

	SYSCALL3(__NR_getresgid, rgid, egid, sgid, ret)

	if (ret >= 0)
		return 0;

	errno = -ret;
	return -1;
}

static int __attribute__((unused))
getresuid(uid_t *ruid, uid_t *euid, uid_t *suid)
{
	int ret;

	SYSCALL3(__NR_getresuid, ruid, euid, suid, ret)

	if (ret >= 0)
		return 0;

	errno = -ret;
	return -1;
}

static uid_t __attribute__((unused))
getuid(void)
{
	uid_t ret;

	SYSCALL0(__NR_getuid, ret)

	return ret;
}

#define TCGETS 0x5401

static int __attribute__((unused))
isatty(int fd)
{
	struct termios term;

	return ioctl(fd, TCGETS, &term) == 0;
}

static int __attribute__((unused))
lchown(const char *path, uid_t owner, gid_t group)
{
	int ret;

	SYSCALL3(__NR_lchown, path, owner, group, ret)

	if (ret >= 0)
		return 0;

	errno = -ret;
	return -1;
}

static int __attribute__((unused))
link(const char *oldpath, const char *newpath)
{
	int ret;

	SYSCALL2(__NR_link, oldpath, newpath, ret)

	if (ret >= 0)
		return 0;

	errno = -ret;
	return -1;
}

#define SEEK_SET  0
#define SEEK_CUR  1
#define SEEK_END  2
#define SEEK_DATA 3
#define SEEK_HOLE 4

static off_t __attribute__((unused))
lseek(int fd, off_t offset, int whence)
{
	off_t ret;

	SYSCALL3(__NR_lseek, fd, offset, whence, ret)

	if (ret >= 0)
		return ret;

	errno = -ret;
	return (off_t)-1;
}

static int __attribute__((unused))
lstat(const char *path, struct stat *buf)
{
	int ret;

	SYSCALL2(__NR_lstat, path, buf, ret)

	if (ret >= 0)
		return 0;

	errno = -ret;
	return -1;
}

static int __attribute__((unused))
mknod(const char *pathname, mode_t mode, dev_t dev)
{
	int ret;

	SYSCALL3(__NR_mknod, pathname, mode, dev, ret)

	if (ret >= 0)
		return 0;

	errno = -ret;
	return -1;
}

static int __attribute__((unused))
pipe(int pipefd[2])
{
	int ret;

	SYSCALL1(__NR_pipe, pipefd, ret)

	if (ret >= 0)
		return 0;

	errno = -ret;
	return -1;
}

static int __attribute__((unused))
pipe2(int pipefd[2], int flags)
{
	int ret;

	SYSCALL2(__NR_pip2, pipefd, flags, ret)

	if (ret >= 0)
		return 0;

	errno = -ret;
	return -1;
}

static ssize_t __attribute__((unused))
read(int fd, void *buf, size_t count)
{
	ssize_t ret;

	SYSCALL3(__NR_read, fd, buf, count, ret)

	if (ret >= 0)
		return ret;

	errno = -ret;
	return -1;
}

static ssize_t __attribute__((unused))
readlink(const char *path, char *buf, size_t bufsiz)
{
	ssize_t ret;

	SYSCALL3(__NR_readlink, path, buf, bufsiz, ret)

	if (ret >= 0)
		return ret;

	errno = -ret;
	return -1;
}

static int __attribute__((unused))
rmdir(const char *pathname)
{
	int ret;

	SYSCALL1(__NR_rmdir, pathname, ret)

	if (ret >= 0)
		return 0;

	errno = -ret;
	return -1;
}

static int __attribute__((unused))
setdomainname(const char *name, size_t len)
{
	int ret;

	SYSCALL2(__NR_setdomainname, name, len, ret)

	if (ret >= 0)
		return 0;

	errno = -ret;
	return -1;
}

static int __attribute__((unused))
setgid(gid_t gid)
{
	int ret;

	SYSCALL1(__NR_setgid, gid, ret)

	if (ret >= 0)
		return 0;

	errno = -ret;
	return -1;
}

static int __attribute__((unused))
sethostname(const char *name, size_t len)
{
	int ret;

	SYSCALL2(__NR_sethostname, name, len, ret)

	if (ret >= 0)
		return 0;

	errno = -ret;
	return -1;
}

static int __attribute__((unused))
setregid(gid_t rgid, gid_t egid)
{
	int ret;

	SYSCALL2(__NR_setregid, rgid, egid, ret)

	if (ret >= 0)
		return 0;

	errno = -ret;
	return -1;
}

static int __attribute__((unused))
setresgid(gid_t rgid, gid_t egid, gid_t sgid)
{
	int ret;

	SYSCALL3(__NR_setresgid, rgid, egid, sgid, ret)

	if (ret >= 0)
		return 0;

	errno = -ret;
	return -1;
}

static int __attribute__((unused))
setresuid(uid_t ruid, uid_t euid, uid_t suid)
{
	int ret;

	SYSCALL3(__NR_setresuid, ruid, euid, suid, ret)

	if (ret >= 0)
		return 0;

	errno = -ret;
	return -1;
}

static int __attribute__((unused))
setreuid(uid_t ruid, uid_t euid)
{
	int ret;

	SYSCALL2(__NR_setreuid, ruid, euid, ret)

	if (ret >= 0)
		return 0;

	errno = -ret;
	return -1;
}

static int __attribute__((unused))
setuid(uid_t uid)
{
	int ret;

	SYSCALL1(__NR_setuid, uid, ret)

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

static int __attribute__((unused))
stat(const char *path, struct stat *buf)
{
	int ret;

	SYSCALL2(__NR_stat, path, buf, ret)

	if (ret >= 0)
		return 0;

	errno = -ret;
	return -1;
}

static int __attribute__((unused))
symlink(const char *oldpath, const char *newpath)
{
	int ret;

	SYSCALL2(__NR_symlink, oldpath, newpath, ret)

	if (ret >= 0)
		return 0;

	errno = -ret;
	return -1;
}

static void __attribute__((unused))
sync(void)
{
	int ret;

	SYSCALL0(__NR_sync, ret)
}

static int __attribute__((unused))
unlink(const char *pathname)
{
	int ret;

	SYSCALL1(__NR_unlink, pathname, ret)

	if (ret >= 0)
		return 0;

	errno = -ret;
	return -1;
}

static ssize_t __attribute__((unused))
write(int fd, const void *buf, size_t count)
{
	ssize_t ret;

	SYSCALL3(__NR_write, fd, buf, count, ret)

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

	SYSCALL3(__NR_ioctl, d, request, arg, ret)

	if (ret >= 0)
		return ret;

	errno = -ret;
	return -1;
}

/* sys/mman.h */

#define PROT_READ  0x1
#define PROT_WRITE 0x2
#define PROT_EXEC  0x4
#define PROT_NONE  0x0

#define MAP_SHARED  0x01
#define MAP_PRIVATE 0x02

#define MAP_32BIT         0x40
#define MAP_ANONYMOUS     0x20
#define MAP_ANON          MAP_ANONYMOUS
#define MAP_DENYWRITE     0 /* ignored */
#define MAP_EXECUTABLE    0 /* ignored */
#define MAP_FILE          0 /* ignored */
#define MAP_FIXED         0x10
#define MAP_GROWSDOWN     0x0100
#define MAP_HUGETLB       0x40000
#define MAP_LOCKED        0x2000
#define MAP_NONBLOCK      0x10000
#define MAP_NORESERVE     0x4000
#define MAP_POPULATE      0x8000
#define MAP_STACK         0x20000
#define MAP_UNINITIALIZED 0x4000000

static void * __attribute__((unused))
mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset)
{
	void *ret;

	register uint64_t __flags  asm("r10") = (uint64_t)flags;
	register uint64_t __fd     asm("r8" ) = (uint64_t)fd;
	register uint64_t __offset asm("r9" ) = (uint64_t)offset;
	asm volatile ("movq $9, %%rax \n\t"
		"syscall"
		: "=a" (ret)
		: "D" (addr), "S" (length), "d" (prot), "r" (__flags),
		  "r" (__fd), "r" (__offset)
		: "%rcx", "%r11", "cc", "memory");

	if ((ssize_t)ret >= 0)
		return ret;

	errno = -(ssize_t)ret;
	return (void *)-1;
}

static int __attribute__((unused))
munmap(void *addr, size_t length)
{
	int ret;

	SYSCALL2(__NR_munmap, addr, length, ret)

	if (ret >= 0)
		return 0;

	errno = -ret;
	return -1;
}

/* sys/resource.h */

#define PRIO_PROCESS (0)
#define PRIO_PGRP    (1)
#define PRIO_USER    (2)

#define PRIO_MIN (-20)
#define PRIO_MAX ( 20)

static int __attribute__((unused))
getpriority(int which, int who)
{
	int ret;

	SYSCALL2(__NR_getpriority, which, who, ret)

	/* returns 40..1 instead of -20..19 to avoid neg. return values */
	if (ret >= 1)
		return -ret + 20;

	errno = -ret;
	/* bogus return value, caller needs to check errno */
	return -1;
}

static int __attribute__((unused))
setpriority(int which, int who, int prio)
{
	int ret;

	SYSCALL3(__NR_setpriority, which, who, prio, ret)

	if (ret >= 0)
		return 0;

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

static int __attribute__((unused))
chmod(const char *path, mode_t mode)
{
	int ret;

	SYSCALL2(__NR_chmod, path, mode, ret)

	if (ret >= 0)
		return 0;

	errno = -ret;
	return -1;
}

static int __attribute__((unused))
fchmod(int fd, mode_t mode)
{
	int ret;

	SYSCALL2(__NR_fchmod, fd, mode, ret)

	if (ret >= 0)
		return 0;

	errno = -ret;
	return -1;
}

static int __attribute__((unused))
mkdir(const char *pathname, mode_t mode)
{
	int ret;

	SYSCALL2(__NR_mkdir, pathname, mode, ret)

	if (ret >= 0)
		return 0;

	errno = -ret;
	return -1;
}

static mode_t __attribute__((unused))
umask(mode_t mask)
{
	mode_t ret;

	SYSCALL1(__NR_umask, mask, ret)

	return ret;
}

/* sys/sysinfo.h */

struct sysinfo {
	long uptime;
	unsigned long loads[3];
	unsigned long totalram;
	unsigned long freeram;
	unsigned long sharedram;
	unsigned long bufferram;
	unsigned long totalswap;
	unsigned long freeswap;
	unsigned short procs;
	unsigned long totalhigh;
	unsigned long freehigh;
	unsigned int mem_unit;
	/* padding is zero on x86_64 */
	/*char _f[20-2*sizeof(long)-sizeof(int)];*/
};

static int __attribute__((unused))
sysinfo(struct sysinfo *info)
{
	int ret;

	SYSCALL1(__NR_sysinfo, info, ret)

	if (ret >= 0)
		return 0;

	errno = -ret;
	return -1;
}

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

	SYSCALL1(__NR_uname, buf, ret)

	if (ret >= 0)
		return 0;

	errno = -ret;
	return -1;
}

#endif
