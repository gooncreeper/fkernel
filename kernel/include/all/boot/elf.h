#ifndef ELF_H
#define ELF_H

#include <boot/exec.h>
#include <binary.h>

enum { ELF_CLASS_32 = 1 ,
       ELF_CLASS_64 = 2};
       
enum { ELF_ENDIAN_LITTLE = 1 ,
       ELF_ENDIAN_BIG    = 2};

enum { ELF_ABI_SYSTEMV = 0};

enum { ELF_TYPE_RELOCATABLE = 1 ,
       ELF_TYPE_EXECUTABLE  = 2 ,
       ELF_TYPE_SHARED      = 3 ,
       ELF_TYPE_CORE_DUMP   = 4};
       
enum { ELF_ISA_X86     = 0x03 ,
       ELF_ISA_X86_64  = 0x3E ,
       ELF_ISA_ARM     = 0x28 ,
       ELF_ISA_AARCH64 = 0xB7 ,
       ELF_ISA_RISC_V  = 0xF3};

// ELF identifier
#define ELF_MAGIC "\x7FELF"

// ELF files are not read via the following structure, since 
// endian and some types vary based on the identifier.

struct elf_header {
	char ei_mag[4];
	u8 ei_abi;
	u8 ei_abi_version;

	u16 e_type;
	u16 e_machine;
	u32 e_version;
	word_t e_entry;
	word_t e_phoff;
	word_t e_shoff;
	u32 e_flags;
	u16 e_ehsize;
	u16 e_phentsize;
	u16 e_phnum;
	u16 e_shentsize;
	u16 e_shnum;
	u16 e_shstrndx;
};

struct elf_program_header {
	u32 p_type;
	u32 p_flags;
	word_t p_offset;
	word_t p_vaddr;
	word_t p_paddr;
	word_t p_filesz;
	word_t p_memsz;
	word_t p_align;
};

#ifdef ELF
	enum executable_type is_elf_binary(struct memory_segment bin);
	void load_elf_binary(struct memory_segment bin);
#else
	#define is_elf_binary(_0) ((enum executable_type)EXECUTABLE_TYPE_NONE)
	#define load_elf_binary(_0) ((void)0)
#endif

#endif //ELF_H
