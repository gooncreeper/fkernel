#ifdef DEBUG_OUTPUT

#include <serial/serial.h>

#include <serial/portio.h>

static void wait_line_status(port_t port, u8 bits) {
	port_t status_port = port + 5;
	u8 current_status;

	do {
		current_status = port_in8(status_port);
		current_status &= bits;
	} while (current_status != bits);
}

void serial_init(port_t port) {
	wait_line_status(port, 0x60);

	port_out8(port + 1, 0x00); // Disable all interrupts
	port_out8(port + 3, 0x80); // Enable DLAB to change Baud Rate
	port_out8(port    , 0x01); // Baud Rate (1) low bit
	port_out8(port + 1, 0x00); //              high bit
	port_out8(port + 3, 0x03); // 8 bit, 1 stop bit, no parity
	port_out8(port + 2, 0xC7);
	port_out8(port + 4, 0x0F); // DTR RTS OUT1 OUT2

	port_in8(port    );
	port_in8(port + 5);
}

void serial_write(port_t port, u8 data) {
	wait_line_status(port, 0x20); // Wait until data can be sent
	port_out8(port, data);
}

u8 serial_read(port_t port) {
	wait_line_status(port, 0x01); // Wait until data arrives
	return port_in8(port);
}

#endif
