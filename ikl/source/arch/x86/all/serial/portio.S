#ifdef DEBUG_OUTPUT

#include <asm.h>

ASM_GLOBAL_FUNC(port_out8)
	movw 4(%esp), %dx
	movb 8(%esp), %al
	outb %al, %dx
	ret

ASM_GLOBAL_FUNC(port_out16)
	movw 4(%esp), %dx
	movw 8(%esp), %ax
	outw %ax, %dx
	ret

ASM_GLOBAL_FUNC(port_out32)
	movw 4(%esp), %dx
	movl 8(%esp), %eax
	outl %eax, %dx
	ret

ASM_GLOBAL_FUNC(port_in8)
	movw 4(%esp), %dx
	inb %dx, %al
	ret

ASM_GLOBAL_FUNC(port_in16)
	movw 4(%esp), %dx
	inw %dx, %ax
	ret

ASM_GLOBAL_FUNC(port_in32)
	movw 4(%esp), %dx
	inl %dx, %eax
	ret

#endif
