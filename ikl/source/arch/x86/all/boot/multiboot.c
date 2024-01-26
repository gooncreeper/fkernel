#ifdef MULTIBOOT

#include <boot/multiboot.h>
#include <boot/mmap.h>
#include <boot/bootinfo.h>
#include <boot/strlen.h>

#include <serial/debug.h>

#include <memory.h>
#include <binary.h>

void multiboot_mmap(struct multiboot_info *mbi) {
	debug_out("multiboot: parsing mmap\n");
	
		// Validate mmap data is passed
	if ((mbi->flags & MULTIBOOT_FLAG_MMAP) == 0) {
		debug_out("failed to boot: no mmap passed\n");
		kexit();
	}
	
		// Get size of multiboot mmap
	size_t buffer_size = mbi->mmap_length;
	debug_format_out("multiboot: mmap size of %zd\n", buffer_size);

		// Regions passed by multiboot
	addr_t region_buffer = (addr_t)mbi->mmap_addr;
	/* We don't use a pointer since a entry's size is variable and we
	need to increment address by the passed size */

		// Enter them into the mmap structure
	debug_format_out("multiboot: reading mmap at 0x%ah\n", region_buffer);
	for (size_t buffer_offset = 0; buffer_offset < buffer_size;) {
			// Get the actual region data passed by multiboot
		addr_t region_addr = region_buffer + buffer_offset;
		struct multiboot_mmap_region *region = (struct multiboot_mmap_region*)region_addr;

			// Debug info
		debug_format_out("multiboot: mmap region\n"
				 " Start: 0x%ah\n Length: 0x%zh\n Type: %id\n", 
		                 (addr_t)region->base_addr,
		                 (size_t)region->length,
		                 (int)region->type
		                );

			// Increment position of buffer based of passed mmap entry size
		buffer_offset += sizeof(region->size) + region->size;

			// Convert passed mmap entry to a standard one
		struct mmap_segment segment;
		segment.memory.start = (addr_t)region->base_addr;
		segment.memory.end   = (addr_t)region->base_addr + region->length;
		segment.type = MMAP_SEG_VOLATILE;

			// Change to type usable if it is
		if (region->type == MULTIBOOT_REGION_TYPE_RAM ||
		    region->type == MULTIBOOT_REGION_TYPE_ACPI)
		    	segment.type = MMAP_SEG_USABLE;
		
			// Add to the mmap
		add_mmap_segment(segment);
	}
}

static void multiboot_modules(struct multiboot_info *mbi) {
	
}

	// Adds a bootinfo entry from the multiboot header.
	//  Additionally, checks the flag for that entry is set.
static void multiboot_add_header_bootinfo(struct multiboot_info *mbi, int flag_bit,
                                          addr_t start, size_t size, 
                                          enum bootinfo_entry_type type) {
		// Check flag is set
	if ((mbi->flags & flag_bit) == 0) {
		debug_out("Flag not set\n");
		return;
	}

		// Debug
	debug_format_out(" Address: %ad\n Length: %zd\n", start, size);

		// Add entry
	struct bootinfo_entry entry =
		{ .start = start,
		  .size  = size,
		  .type  = type,
		  .type_specific = 0
		};
	add_bootinfo_entry(entry);
}

void multiboot_bootinfo(struct multiboot_info *mbi) {
		// Multiboot Header
	struct bootinfo_entry header =
		{ .start = (addr_t)mbi,
		  .size  = 172,
		  .type  = BOOTINFO_ENTRY_MULTIBOOT_HEADER,
		  .type_specific = 0
		};
	add_bootinfo_entry(header);

		// Command Line string
	addr_t cmdline_addr   = (addr_t)mbi->cmdline;
	size_t cmdline_length = strlen((char*)cmdline_addr);
	
	multiboot_add_header_bootinfo(mbi,
		MULTIBOOT_FLAG_CMDLINE,
		cmdline_addr,
		cmdline_length,
		BOOTINFO_ENTRY_MULTIBOOT_CMDLINE);

		// Modules
	multiboot_add_header_bootinfo(mbi,
		MULTIBOOT_FLAG_MODULES,
		(addr_t)mbi->mods_addr,
		(size_t)mbi->mods_count * sizeof(struct multiboot_module),
		BOOTINFO_ENTRY_MULTIBOOT_MODULES);
	multiboot_modules(mbi);
	
		// mmap
	multiboot_add_header_bootinfo(mbi,
		MULTIBOOT_FLAG_MMAP,
		(addr_t)mbi->mmap_addr,
		(size_t)mbi->mmap_length,
		BOOTINFO_ENTRY_MULTIBOOT_MMAP);
	// processing mmap is not handled here as it is not bootinfo
	//  instead its own mmap thing.

		// Drives
	multiboot_add_header_bootinfo(mbi,
		MULTIBOOT_FLAG_DRIVES,
		(addr_t)mbi->drives_addr,
		(size_t)mbi->drives_length,
		BOOTINFO_ENTRY_MULTIBOOT_DRIVES);

		// Boot Loader Name
	addr_t loader_addr = (addr_t)mbi->boot_loader_name;
	size_t loader_size = sizeof((char*)loader_addr);

	multiboot_add_header_bootinfo(mbi,
		MULTIBOOT_FLAG_BOOT_LOADER_NAME,
		loader_addr,
		loader_size,
		BOOTINFO_ENTRY_MULTIBOOT_BOOT_LOADER_NAME);

		// APM table
	// we will get to it
	// need to parse struct
	// looks like it has other structs pointed too
	// Advancedf Power Management
}

#endif //MULTIBOOT
