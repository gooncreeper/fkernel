/* mmap.h
 * Provides an architecture/platform independent way to store a memory map.
 * Additionally, provides a function to add to the mmap. */

#ifndef MMAP_H
#define MMAP_H

#include <memory.h>

// Maximum number of mmap segments we allocate
// We may need more than passed by boot as we expand some of the segments
#ifndef MAX_MMAP_SEGMENTS
#define MAX_MMAP_SEGMENTS 32
#endif

enum mmap_segment_type { MMAP_SEG_USABLE   ,  // Generic memory,   safe for kernel objects 
                         MMAP_SEG_VOLATILE};  // Special memory, unsafe for kernel objects

struct mmap_segment {
	struct  memory_segment memory;
	enum mmap_segment_type type  ;
};

	// Global mmap
extern size_t used_mmap_segments;
extern struct mmap_segment mmap[MAX_MMAP_SEGMENTS];

	/* Checks if there is free space in mmap and adds the segment
	When it adds the segment, it does three things:
	* Aligns regions to a memory_block (4 words)
	  * Usable   memory shrinks  inwards in allignemnt
	  * Volatile memory expands outwards in allignment
	* Merges overlapping / consecutive regions of the same type
	* Ignores regions of size 0 */
void add_mmap_segment(struct mmap_segment segment);

	/* Does some final fixes to the mmap
		More specifically:
	It splits memory segments overlapping with volatile memory. */
void finalize_mmap(void);

#endif //MMAP_H
