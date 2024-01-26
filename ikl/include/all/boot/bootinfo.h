/* bootinfo.h
 * Structure for architecture/platform dependent information to
 * be stored from boot. */

#ifndef BOOTINFO_H
#define BOOTINFO_H

#include <memory.h>

#ifndef MAX_BOOTINFO_ENTRIES
#define MAX_BOOTINFO_ENTRIES 512
#endif

	/* Type of entry */
enum bootinfo_entry_type { BOOTINFO_ENTRY_MODULE                     = 0x0001 ,
			// type_specific - Module #n
                           BOOTINFO_ENTRY_BOOT_EXECUTABLE            = 0x0081 ,
                           
                           BOOTINFO_ENTRY_MULTIBOOT_HEADER           = 0x0100 ,
                           BOOTINFO_ENTRY_MULTIBOOT_CMDLINE          = 0x0102 ,
                           BOOTINFO_ENTRY_MULTIBOOT_MODULES          = 0x0103 ,
                           BOOTINFO_ENTRY_MULTIBOOT_MMAP             = 0x0106 ,
                           BOOTINFO_ENTRY_MULTIBOOT_DRIVES           = 0x0107 ,
                        // BOOTINFO_ENTRY_MULTIBOOT_CONFIG_TABLE     = 0x0108 ,
                        // Can't find any description of the structure,
                        // it's from a BIOS call, though who knows, it isn't
                        // implemented in GRUB.
                           BOOTINFO_ENTRY_MULTIBOOT_BOOT_LOADER_NAME = 0x0109 ,
                           BOOTINFO_ENTRY_MULTIBOOT_APM_TABLE        = 0x010A ,
                           BOOTINFO_ENTRY_MULTIBOOT_MODULE_STRING    = 0x0183};

struct bootinfo_entry {
	addr_t start;
	size_t size;
	word_t type;
	word_t type_specific;
};

	// Global bootinfo table
extern size_t used_bootinfo_entries;
extern struct bootinfo_entry bootinfo[MAX_BOOTINFO_ENTRIES];

	// Adds a bootinfo entry, also checks if there is enough space
void add_bootinfo_entry(struct bootinfo_entry entry);

#endif //BOOTINFO_H
