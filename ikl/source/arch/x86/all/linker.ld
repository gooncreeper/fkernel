ENTRY(_start)

SECTIONS {
	. = 1M;

	.rodata BLOCK(4K) : ALIGN(4K) {
		*(.multiboot)
		*(.multiboot2)

		*(.rodata)
	}

	.text BLOCK(4K) : ALIGN(4K) {
		*(.text)
	}		

	.data BLOCK(4K) : ALIGN(4K) {
		*(.data)
	}

	.bss BLOCK(4K) : ALIGN(4K) {
		*(COMMON)
		*(.bss)
	}

	/* If the compiler wants to add it's signature comment,
	then it shall. */
}
