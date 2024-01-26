#ifdef DEBUG_OUTPUT

#include <serial/debug.h>

#include <serial/serial.h>
#include <serial/portio.h>

port_t debug_port;

void debug_init() {
	int bda_portn = *(u16*)(word_t)0x410 & 07000; // # of BDA Ports << 9
	// no need to shift port number, just checking if any provided
	debug_port = bda_portn ? *(port_t*)(word_t)0x400 : DEFAULT_DEBUG_PORT;

	serial_init(debug_port);
	debug_out("\ndebug output initialized\n");
}

void debug_char_out(const char c) {
	serial_write(debug_port, (u8)c);
}

void debug_out(const char* data) {
	while(*data) debug_char_out(*data++);
}

#endif
