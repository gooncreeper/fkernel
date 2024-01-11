#ifndef SERIAL_H
#define SERIAL_H

#include <serial/portio.h>
#include <binary.h>

void serial_init(port_t port);
void serial_write(port_t port, u8 data);
u8 serial_read(port_t port);

#endif //SERIAL_H
