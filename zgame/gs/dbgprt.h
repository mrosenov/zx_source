#ifndef __ONLINEGAME_GS_DBGPRT_H__
#define __ONLINEGAME_GS_DBGPRT_H__

#include <stdio.h>

extern bool __PRINT_FLAG;

inline void __SETPRTFLAG(bool flag)
{
	__PRINT_FLAG  = flag;
}

#ifdef __NO_STD_OUTPUT__
inline int __PRINTF(const char * fmt, ...)
{
}
#else
#include <stdarg.h>
inline int __PRINTF(const char * fmt, ...)
#ifdef __GNUC__	
		__attribute__ ((format (printf, 1, 2)))
#endif
;

inline int __PRINTF(const char * fmt, ...)
{
	if(!__PRINT_FLAG) return 0;
	va_list ap;
	va_start(ap, fmt);
	int rst = vprintf(fmt, ap);
	va_end(ap);
	return rst;
}
#endif

#endif

