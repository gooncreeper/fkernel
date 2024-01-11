#ifndef BINARY_H
#define BINARY_H

#include <stdint.h>
#include <memory.h> // must be included after arch_binary, since memory.h requires word_t
#include <word.h>

#define BIT(x) (1 << x)

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

enum endian { LITTLE_ENDIAN, BIG_ENDIAN };

u8  read_u8 (addr_t *addr, enum endian endian);
u16 read_u16(addr_t *addr, enum endian endian);
u32 read_u32(addr_t *addr, enum endian endian);
u64 read_u64(addr_t *addr, enum endian endian); 

#endif //BINARY_H
