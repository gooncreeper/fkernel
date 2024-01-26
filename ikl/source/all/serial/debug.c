#ifdef DEBUG_OUTPUT

#include <stdarg.h>

#include <serial/debug.h>

#include <memory.h>

/* Format String:
	1 Character Formats
%s - string pointer

	2 Character Formats
		Type
%i_ - integer
%w_ - word_t
%a_ - addr_t
%z_ - size_t
		Output
%_b - x size value in binary
%_d - x size value in decimal
%_h - x size value in hexadecimal
*/

#define DEC_CHARACTER(c) ('0' + c)
#define HEX_CHARACTER(c) ('a' + c - 10)
#define BASE_CHARACTER(c) (c < 10 ? DEC_CHARACTER(c) : HEX_CHARACTER(c))

#define DEC_DECODE(c) (c - '0')
#define IS_DECIMAL(c) (c >= '0' && c <= '9')
#define INVALID_FORMAT continue

void debug_format_out(const char *restrict format, ...) {
	va_list va;
	va_start(va, format);

	while (*format) {
		if (*format != '%') {
			debug_char_out(*format++);
			continue;
		}

		if (*++format == 's') {
			char* s= va_arg(va, char*);
			debug_out(s);
			format++;
			continue;
		}

		word_t value;
		size_t value_size;
		switch (*format) {
			case 'i':
				value = va_arg(va, int);
				value_size = sizeof(int);
				break;

			case 'w':
				value = va_arg(va, word_t);
				value_size = sizeof(word_t);
				break;

			case 'a':
				value = va_arg(va, addr_t);
				value_size = sizeof(addr_t);
				break;

			case 'z':
				value = va_arg(va, size_t);
				value_size = sizeof(size_t);
				break;

			default: INVALID_FORMAT;
		}

		int base;
		size_t buffer_size; // default base 10
		switch (*++format) {
			case 'b':
				base = 2;
				buffer_size = value_size * 8;
				break;
				
			case 'd':
				base = 10;
				buffer_size = value_size * 3; // max for a byte is 3 digits
				break;
				
			case 'h':
				base = 16;
				buffer_size = value_size * 2;
				break;
				
			default: INVALID_FORMAT;
		}

		format++;

		char buffer_mem[buffer_size];
		char *buffer = buffer_mem;
		char *buffer_start = buffer;
		
		do { // atleast one iteration for zero
			*buffer++ = BASE_CHARACTER(value % base);
			value /= base;
		} while (value);

		// note: we put the digit in reverse
		// therefore we need to write them in reverse
		while (--buffer >= buffer_start)
			debug_char_out(*buffer);
	}

	va_end(va);
}

#endif
