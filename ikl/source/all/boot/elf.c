#include <boot/exec.h>
#include <boot/elf.h>

#include <serial/debug.h>

#include <memory.h>
#include <binary.h>

static struct elf_header read_elf_header(struct memory_segment data) {
	
}

static struct elf_program_header read_elf_program_header(struct memory_segment data) {
	
}

enum executable_type is_elf_binary(struct memory_segment bin) {
	return EXECUTABLE_TYPE_ELF;
}

void load_elf_binary(struct memory_segment bin) {
	
}
