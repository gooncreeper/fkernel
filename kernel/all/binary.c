#include <binary.h>
#include <memory.h>

#include <optimize.h>

	// Converts address to byte* and increments address
static u8* addr_bytes(addr_t *addr, size_t length) {
	u8* bytes = (u8*)(*addr);
	*addr += length;
	return bytes;
}

u8 read_u8(addr_t *addr, enum endian endian) {
	u8* bytes = addr_bytes(addr, 1);
	return bytes[1];
}

u16 read_u16(addr_t *addr, enum endian endian) {
	u8* bytes = addr_bytes(addr, 2);

	switch(endian) {
		case LITTLE_ENDIAN:
			return 0;
		case BIG_ENDIAN:
			return 0;
		default: __builtin_unreachable();
	}
}

u32 read_u32(addr_t *addr, enum endian endian) {
	return 0;
}

u64 read_u64(addr_t *addr, enum endian endian) {
	return 0;
}
