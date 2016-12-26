all : $(UTILS)

ARCH ?= $(shell uname -m)

ifeq ($(ARCH),x86_64)
UTILS = true false
CFLAGS = -std=c99 -pedantic -Wall -Wextra -Os -pipe \
	 -fno-unwind-tables -fno-asynchronous-unwind-tables
STRIP = strip
EXE = $(patsubst %,x86_64/%,$(UTILS))
endif

ifeq ($(ARCH),x86_64)
all : $(EXE)

x86_64/%.o : %.c lib.h
	[ -d x86_64 ] || mkdir x86_64
	$(CC) $(CFLAGS) -c -nostdlib -ffreestanding $< -o $@

x86_64/% : x86_64/%.o start_x86_64.o
	$(LD) start_x86_64.o $< -o $@
	$(STRIP) -R .comment $@
endif

start_x86_64.o : start_x86_64.s
	$(AS) $< -o $@

cppcheck :
	cppcheck --enable=all *.c

clean :
	rm -f *.o
	rm -rf x86_64/

.PHONY : clean cppcheck
