#include <boot/mmap.h>
#include <memory.h>

#include <serial/debug.h>

size_t used_mmap_segments = 0;
struct mmap_segment mmap[MAX_MMAP_SEGMENTS];

	#ifdef DEBUG_OUTPUT
static void debug_mmap_segment(struct mmap_segment segment) {
	debug_format_out(" Start: %ah\n End: %ah\n Type: %id\n",
                         segment.memory.start,
                         segment.memory.end,
                         segment.type);
}
	#else
#define output_mmap_segment(_0)
	#endif

	/* Adds a new segment to the mmap
	Additionally checks if we have enough free room,
	and skips the segment if it is of size 0. */
static void new_mmap_segment(struct mmap_segment segment) {
	debug_out("mmap: Adding new mmap segment\n");
	debug_mmap_segment(segment);

		// Check that there is actually any memory
	if (segment.memory.start >= segment.memory.end) {
		debug_out("mmap: Size of 0, ignoring segment\n");
		return;
	}

		// Check we have room to add segment
	if (used_mmap_segments == MAX_MMAP_SEGMENTS) {
		debug_out("mmap: No free room in mmap, segment voided\n");
		return;
	}

		// Add segment
	mmap[used_mmap_segments++] = segment;
}

	/* Removes an index and shifts others in its place */
static void remove_mmap_segment(size_t mmap_index) {
		// Debug
	debug_out("mmap: Removing mmap segment\n");
	debug_mmap_segment(mmap[mmap_index]);

		// Mark that the size has decreased
	used_mmap_segments--;

		// Shift all the other segments
	for (; mmap_index < used_mmap_segments; mmap_index++) {
		/* mmap_index will stop one before the last index
		as we decremented used_mmap_segments earlier */
		mmap[mmap_index] = mmap[mmap_index + 1];
	}
}

	// Align an address to it's closest position towards 0
static addr_t align_lower(addr_t position) {
	return position & ~(sizeof(struct memory_block) - 1);
}

	// Align an address to it's closest position towards infinity
static addr_t align_higher(addr_t position) {
	return align_lower(position + sizeof(struct memory_block) - 1);
}

	// Align with unalligned memory truncated
static void align_memory_inwards(struct memory_segment *memory) {
	memory->start = align_higher(memory->start);
	memory->end   = align_lower (memory->end  );
}

	// Align with unalligned memory expanded
static void align_memory_outwards(struct memory_segment *memory) {
	memory->start = align_lower (memory->start);
	memory->end   = align_higher(memory->end  );
}

	// Aligns a mmap segment to 16 byte boundries
static void align_segment(struct mmap_segment *segment) {
		// Debug
	debug_out("mmap: Aligning segment\n");

		// Align in direction based on type
	switch (segment->type) {
		case MMAP_SEG_USABLE:
			align_memory_inwards(&segment->memory);
			break;
		case MMAP_SEG_VOLATILE:
			align_memory_outwards(&segment->memory);
			break;
		default:
			__builtin_unreachable();
	}
}

	// Returns if a memory segment can merge with another
static int mem_mergable(struct memory_segment memory, struct memory_segment testing) {
	return testing.end >= memory.start && testing.start <= memory.end;
}

static addr_t min_addr(addr_t a, addr_t b) {
	return (a < b) ? a : b;
}

static addr_t max_addr(addr_t a, addr_t b) {
	return (a > b) ? a : b;
}

	// Merges two memory segments by finding the minimum and maximum of the start & ends
static void mmap_merge(struct memory_segment *target, struct memory_segment with) {
	target->start = min_addr(target->start, with.start);
	target->end   = max_addr(target->end  , with.end  );
}

	/* Merges any clean segments of the same type, either consecutive or overlapping */
static void mmap_optimize_consecutive(struct mmap_segment *segment) {
	debug_out("mmap: Attempting to merge segments with others\n");

		// Go through each possible segment
	for (size_t i = 0; i < used_mmap_segments; i++) {
		struct mmap_segment testing = mmap[i];

		debug_out("mmap: Testing segment\n");
		debug_mmap_segment(testing);
	
			// Ensure they are the same type
		if (segment->type != testing.type) {
			debug_out("mmap: Not same type, skipping\n");
			continue;
		}

			// Check if we can merge them at the start
		if (mem_mergable(segment->memory, testing.memory)) {
			debug_out("mmap: Merging segments\n");
			mmap_merge(&segment->memory, testing.memory);
			// Since we are removing the current one we are testing,
			// we have to go over this index again (i--)
			remove_mmap_segment(i--);
		} else {
			debug_out("mmap: Cannot merge segments\n");
		}
	}
}

	/* Adds a new segment, only one time changes are applied to it
	Applied changes: Alignment, Merging
	 Volatile overlaps aren't handled as splitting leads to an indefinite amount of 
	entries to handle, leading to required recursion. */
void add_mmap_segment(struct mmap_segment segment) {
		// Debug
	debug_out("mmap: Adding new mmap segment\n");
	debug_mmap_segment(segment);

		// Clean
	align_segment(&segment);
	mmap_optimize_consecutive(&segment);

		// More debug
	debug_out("mmap: Final segment state\n");
	debug_mmap_segment(segment);

		// Adds a new segment
	new_mmap_segment(segment);
}

	// Returns if two memory segments are overlapping
static int mem_overlapping(struct memory_segment memory, struct memory_segment testing) {
	return testing.end > memory.start && testing.start < memory.end;
}

	// Splits a memory segment
static void mmap_split(size_t segment_index, struct memory_segment split) {
		// Get segment we are splitting
	struct mmap_segment segment = mmap[segment_index];

		// Debug info
	debug_format_out("mmap: Spliting memory at\n Start: %ah\n End: %ah\n",
	                 split.start, split.end);

		// Two new segments from split
	struct mmap_segment  lowerSegment = segment;
	struct mmap_segment higherSegment = segment;

		// Splitting is actually pretty simple
	 lowerSegment.memory.end = split.start;
	higherSegment.memory.start = split.end;
	/* Regions may be a negative size, but luckily new_mmap_segment handles that */

		// Debug
	debug_out("mmap: Split lower\n");
	debug_mmap_segment(lowerSegment);

	debug_out("mmap: Split higher\n");
	debug_mmap_segment(higherSegment);

		// Remove old and add new
	remove_mmap_segment(segment_index);
	new_mmap_segment( lowerSegment);
	new_mmap_segment(higherSegment);
}

static void mmap_split_overlapping(struct memory_segment memory) {
	debug_format_out("mmap: Searching for overlaps\n");

	for (size_t i = 0; i < used_mmap_segments; i++) {
		struct mmap_segment segment = mmap[i];
		
		debug_out("mmap: Checking segment\n");
		debug_mmap_segment(segment);

		if (segment.type != MMAP_SEG_USABLE) {
			debug_out("mmap: Segment is volatile\n");
			continue;
		}

		if (mem_overlapping(memory, segment.memory)) {
			//i--; have to go over index again
			mmap_split(i--, memory);
			continue;
		}
	}
}

	/* Finalizes the memory map by splitting memory segments with volatile memory inside them */
void finalize_mmap(void) {
	debug_out("mmap: Finalizing memory map - cleaning volatile overlaps\n");

	for (size_t i = 0; i < used_mmap_segments; i++) {
		struct mmap_segment segment = mmap[i];

		debug_out("mmap: Checking segment\n");
		debug_mmap_segment(segment);

		if (segment.type != MMAP_SEG_VOLATILE) {
			debug_out("mmap: Segment is not volatile\n");
			continue;
		}

		mmap_split_overlapping(segment.memory);
	}

	debug_out("mmap: Final mmap\n");
	for (size_t i = 0; i < used_mmap_segments; i++) {
		debug_format_out("mmap: Segment %id\n", i);
		debug_mmap_segment(mmap[i]);
	}
}
