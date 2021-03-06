ARCH ?= $(shell uname -m)

ifeq ($(ARCH),x86_64)
UTILS = true false yes clear echo printenv pwd uname arch \
	hostname sleep chvt reset basename chroot domainname \
	__reboot __poweroff dirname mesg sync nice renice \
	dmesg rmdir cat cmp nohup __sh tee env
CFLAGS = -std=gnu11 -pedantic -Wall -Wextra -Os -pipe \
	 -fno-unwind-tables -fno-asynchronous-unwind-tables \
	 -Wno-unused-function -fno-stack-protector
STRIP = strip
EXE = $(patsubst %,x86_64/%,$(UTILS))

all : $(EXE)

start_x86_64.o : start_x86_64.s

x86_64/%.o : %.c lib.h lib_x86_64.h syscalls_x86_64.h
	@[ -d x86_64 ] || mkdir x86_64
	$(CC) $(CFLAGS) -c -nostdlib -ffreestanding $< -o $@

x86_64/% : x86_64/%.o start_x86_64.o
	$(LD) --gc-sections start_x86_64.o $< -o $@
	$(STRIP) -R .comment $@
endif

ifeq ($(ARCH),dos)
UTILS = true false
CFLAGS = -std=gnu99 -pedantic -Wall -Wextra -Os -pipe -D__dos__ \
	 -m32 -march=i386 -fno-toplevel-reorder \
	 -fno-unwind-tables -fno-asynchronous-unwind-tables \
	 -Wno-unused-function
ASFLAGS = --32 -march=i386
EXE = $(patsubst %,dos/%.com,$(UTILS))

all : $(EXE)

start_dos.o : start_dos.s

dos/%.o : %.c lib.h
	@[ -d dos ] || mkdir dos
	$(CC) $(CFLAGS) -c -nostdlib -ffreestanding $< -o $@

dos/%.com : dos/%.o start_dos.o
	$(LD) --gc-sections -n -m elf_i386 -T dos.ld start_dos.o $< -o $@
endif

dosbox :
	@dosbox dos &

cppcheck :
	cppcheck --enable=all *.c

clean :
	rm -f *.o
	rm -rf x86_64/
	rm -rf dos/

test : all
	@sh tests.sh

.PHONY : clean cppcheck dosbox test
