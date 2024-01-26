#ifndef EXECUTABLE_H
#define EXECUTABLE_H

#include <memory.h>

enum executable_type { EXECUTABLE_TYPE_NONE ,
                       EXECUTABLE_TYPE_ELF };

struct exec_binary {
	struct memory_segment binary;
	enum  executable_type type;
};

enum executable_type exec_type(struct exec_binary bin);
void                 exec_load(struct exec_binary bin);

#endif //EXECUTABLE_H
