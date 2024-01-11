#include <boot/boot.h>
#include <boot/mmap.h>
#include <boot/bootinfo.h>

void generic_start(void) {
		// Finalize the good old mmap
	finalize_mmap();

	kexit();
}
