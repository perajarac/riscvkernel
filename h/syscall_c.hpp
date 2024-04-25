#ifndef _syscall_c_hpp_
#define _syscall_c_hpp_

#include "../h/riscv.hpp"


void* mem_alloc(size_t size);
int mem_free (void*);

class _thread;
typedef _thread* thread_t;

int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg);
int thread_exit();
void thread_dispatch();

#endif