#ifndef _syscall_c_h_
#define _syscall_c_h_

#include "../h/riscv.hpp"

void* mem_alloc(size_t size);
int mem_free (void*);

class TCB;
typedef TCB* thread_t;

int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg);
int thread_exit();
void thread_dispatch();

class KernelSemaphore;
typedef KernelSemaphore* sem_t;

int sem_open(sem_t* handle, unsigned init);
int sem_close(sem_t handle);
int sem_wait(sem_t id);
int sem_signal(sem_t id);
int sem_trywait(sem_t id);

typedef unsigned long time_t;

int time_sleep (time_t);
int sem_timedwait(
 sem_t id,
 time_t timeout
);

const int EOF = -1;
char getc();
void putc(char);

#endif