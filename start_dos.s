.text
.code16
.globl start

start:
	call main

	# exit with error code (al)
	movb $0x4c, %ah
	int $0x21
