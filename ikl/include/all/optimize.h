#ifndef OPTIMIZE_H
#define OPTIMIZE_H

#ifdef NO_BUILTIN_UNREACHABLE
	#define __builtin_unreachable() kexit()
#endif

#endif //OPTIMIZE_H
