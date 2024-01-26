#ifndef DEBUG_H
#define DEBUG_H

#define DEFAULT_DEBUG_PORT 0x3F8

	#ifdef DEBUG_OUTPUT
void debug_init(void);

void debug_format_out(const char *restrict format, ...);
void debug_char_out(const char c);
void debug_out(const char* data);

	#else // void macros
#define debug_init() ((void)0)

#define debug_format_out(_0, ...) ((void)0)
#define debug_char_out  (_0) ((void)0)
#define debug_out       (_0) ((void)0)
	#endif

#endif //DEBUG_H
