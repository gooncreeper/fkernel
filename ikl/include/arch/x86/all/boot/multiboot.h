#ifndef MULTIBOOT_H
#define MULTIBOOT_H

#include <boot/boot.h>
#include <boot/exec.h>
#include <binary.h>

#define MULTIBOOT_BOOTLOADER_MAGIC 0x2BADB002

#define MULTIBOOT_FLAG_CMDLINE          BIT(2)
#define MULTIBOOT_FLAG_MODULES          BIT(3)
#define MULTIBOOT_FLAG_MMAP             BIT(6)
#define MULTIBOOT_FLAG_DRIVES           BIT(7)
#define MULTIBOOT_FLAG_CONFIG_TABLE     BIT(8)
#define MULTIBOOT_FLAG_BOOT_LOADER_NAME BIT(9)
#define MULTIBOOT_FLAG_APM_TABLE        BIT(10)

enum multiboot_region_type {
	MULTIBOOT_REGION_TYPE_RAM  = 1,
	MULTIBOOT_REGION_TYPE_ACPI = 3,
};

struct multiboot_mmap_region {
	u32 size; // Size of the mmap entry
	u64 base_addr;
	u64 length; // Length of the mmap segment
	u32 type;
} PACKED_STUCT;

struct multiboot_module {
	u32 mod_start;
	u32 mod_end;
	u32 mod_string;
	u32 reserved;
} PACKED_STRUCT;

struct multiboot_info {
	u32 flags;

	u32 mem_lower;
	u32 mem_upper;

	u32 boot_device;

	u32 cmdline;

	u32 mods_count;
	u32 mods_addr;

	u32 sym[4]; // UNUSED

	u32 mmap_length;
	u32 mmap_addr;

	u32 drives_length;
	u32 drives_addr;

	u32 config_table;

	u32 boot_loader_name;

	u32 apm_table;
} PACKED_STRUCT;

void multiboot_mmap    (struct multiboot_info *mbi);
void multiboot_bootinfo(struct multiboot_info *mbi);

#endif //MULTIBOOT_H
