#ifndef printf_h_
#define printf_h_
#include "../lib/console.h"

/* printing int, long, string,   char,  hexa value
            %d,    %l,     %s,    %c    %x
*/
extern void printf(const char* format, void* argument= 0);

#endif