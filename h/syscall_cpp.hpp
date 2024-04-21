#ifndef _syscall_cpp_h_
#define _syscall_cpp_h_

#include "../h/syscall_c.hpp"

void* operator new(size_t size);
void operator delete(void* addr);

void* operator new[](size_t size);
void operator delete[](void* addr);



#endif