#ifndef PORTIO_H
#define PORTIO_H

#include <binary.h>

typedef u16 port_t;

void port_out8 (port_t port,  u8 data);
void port_out16(port_t port, u16 data);
void port_out32(port_t port, u32 data);

u8  port_in8 (port_t port);
u16 port_in16(port_t port);
u32 port_in32(port_t port);

#endif //PORTIO_H
