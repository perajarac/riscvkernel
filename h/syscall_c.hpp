#ifndef _syscall_c_hpp_
#define _syscall_c_hpp_

#include "../h/riscv.hpp"
#include "../h/memoryallocator.hpp"


void* mem_alloc(size_t size);
int mem_free (void*);





#endif