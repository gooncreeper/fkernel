#include <boot/boot.h>

#include <boot/multiboot.h>
#include <boot/multiboot2.h>

#include <serial/debug.h>

void arch_start(int mbj, void* mbi) {
		// Initialize debug output
	debug_init();

		// Boot specific handler
	switch(mbj) {
#ifdef MULTIBOOT
	case MULTIBOOT_BOOTLOADER_MAGIC:
		debug_out("multiboot: parsing info\n");
	
		struct multiboot_info *multiboot_info = (struct multiboot_info*)mbi;
		multiboot_mmap    (multiboot_info);
		multiboot_bootinfo(multiboot_info);

		break;
#endif

#ifdef MULTIBOOT2
	case MULTIBOOT2_BOOTLOADER_MAGIC:
		debug_out("multiboot2: not supported (YET)\n")
		kexit();
		break;
#endif

	default:
		debug_out("failed to boot: no known boot magic passed\n");
		kexit();
		break;
	}

	generic_start();
}
