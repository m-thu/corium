.globl _start

.text
_start:
	# mark as deepest stack frame
	xorq %rbp, %rbp

	# first argument : rdi = argc
	popq %rdi
	# second argument: rsi = argv
	movq %rsp, %rsi
	# third argument : rdx = envp = 8*argc + rsp + 8
	leaq 8(%rsi, %rdi, 8), %rdx
	movq %rdx, environ
	# align stack to 16 bytes
	andq $-16, %rsp

	call main

	# sys_exit
	movq %rax, %rdi
	movq $60, %rax
	syscall

.globl environ

.data
environ:
	.quad 0
