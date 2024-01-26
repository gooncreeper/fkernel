#include <boot/exec.h>
#include <boot/elf.h>

#include <optimize.h>

enum executable_type exec_type(struct exec_binary bin) {
	return is_elf_binary(bin.binary); // || others
}

void exec_load(struct exec_binary bin) {
	switch(bin.type) {
		case EXECUTABLE_TYPE_NONE:
			// no binary provided
			break;

		case EXECUTABLE_TYPE_ELF:
			load_elf_binary(bin.binary);
			break;

		default: __builtin_unreachable();
	}
}
