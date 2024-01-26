#include <asm.h>

.section .bss
	# Define where stack is
	.align 16

	stack_botton:
	.skip 4096 # Stack size - 4 KiB
	stack_top:

.section .text
		# Program entry
	ASM_GLOBAL_FUNC(_start)
		# C Stack
	mov $stack_top, %esp

	# System V i386
	push %ebx
	push %eax
		
	call arch_start

	ASM_GLOBAL_FUNC(kexit)
	cli
ih:	hlt
	jmp ih
