#ifdef MULTIBOOT

.set MULTIBOOT_MAGIC, 0x1BADB002
.set MULTIBOOT_FLAGS, 0x3 # 4k Module Alignment and Memory Map

.section .multiboot
	.align 4
	.long MULTIBOOT_MAGIC
	.long MULTIBOOT_FLAGS
	.long -(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS)

#endif

#ifdef MULTIBOOT2

.section .multiboot2

#endif
