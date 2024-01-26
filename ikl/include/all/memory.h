#ifndef MEMORY_H
#define MEMORY_H

#include <word.h>

#define PACKED_STRUCT __attribute__((packed))

typedef word_t addr_t;
typedef word_t size_t;

	// Our size_t may vary from the compiler's
#define sizeof(x) ((size_t)sizeof(x))

struct memory_segment {
	addr_t start; // INCLUSIVE
	addr_t end  ; // EXCLUSIVE
};

struct memory_block {
	word_t word[4];
};

#endif //BINARY_H
