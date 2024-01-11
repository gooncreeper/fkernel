#include <boot/bootinfo.h>

#include <serial/debug.h>

size_t used_bootinfo_entries = 0;
struct bootinfo_entry bootinfo[MAX_BOOTINFO_ENTRIES];

	// Adds an entry to boot info, very simple
void add_bootinfo_entry(struct bootinfo_entry entry) {
		// Print the awesome debug information
	debug_format_out("Adding bootinfo entry\n"
	                 "Start: %ah\n Size: %zh\n Type: %wh\n Type Specific: %wd\n",
	                 entry.start, entry.size, entry.type, entry.type_specific);

		// Check we have space
	if (used_bootinfo_entries == MAX_BOOTINFO_ENTRIES) {
		debug_out("No free room in bootinfo table, entry voided\n");
		return;
	}

	bootinfo[used_bootinfo_entries++] = entry;
}
