#include "../h/syscall_c.h"

void* mem_alloc(size_t size) {
    uint64 volatile ssize = (size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;

    __asm__ volatile("mv a1, %0" : : "r" (ssize));
    __asm__ volatile("mv a0, %0" : : "r" (0x01));
    __asm__ volatile("ecall");

    uint64 volatile ret;
    __asm__ volatile("mv %0, a0" : "=r" (ret));
    return (void*)ret;
}

int mem_free(void* addr)
{
    __asm__ volatile("mv a1, %0" : : "r" ((uint64)addr));
    __asm__ volatile("mv a0, %0" : : "r" (0x02));
    __asm__ volatile("ecall");

    uint64 volatile ret;
    __asm__ volatile("mv %0, a0" : "=r" (ret));
    return (int)ret;
}
int thread_create(thread_t* handle, void (*start_routine)(void*), void* arg) {

    void* volatile stack_space = (void*)MemoryAllocator::kernel_mem_alloc(BLOCKS_FOR_STACK_SIZE);  //64 blocks is default stack size
    if(!stack_space) 
        return -1;

    __asm__ volatile("mv a4, %0" : : "r" ((uint64)stack_space));
    __asm__ volatile("mv a3, %0" : : "r" ((uint64)arg));
    __asm__ volatile("mv a2, %0" : : "r" ((uint64)start_routine));
    __asm__ volatile("mv a1, %0" : : "r" ((uint64)handle));
    __asm__ volatile("mv a0, %0" : : "r" (0x11));

    __asm__ volatile("ecall");

    uint64 volatile ret;
    __asm__ volatile("mv %0, a0" : "=r" (ret));
    return (int)ret;
}

int thread_exit()
{
     __asm__ volatile("mv a0, %0" : : "r" (0x12));
    __asm__ volatile ("ecall");

    uint64 volatile ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return (int)ret;
}

void thread_dispatch() {
    __asm__ volatile("mv a0, %0" : : "r" (0x13));
    __asm__ volatile("ecall");
}

int sem_open(sem_t *handle, unsigned init)
{
    __asm__ volatile("mv a2, %0" : : "r" ((uint64)init));
    __asm__ volatile("mv a1, %0" : : "r" ((uint64)handle));
    __asm__ volatile("mv a0, %0" : : "r" (0x21));

    __asm__ volatile("ecall");

    uint64 volatile ret;
    __asm__ volatile("mv %0, a0" : "=r" (ret));
    return (int)ret;
    
}

int sem_close(sem_t handle)
{
    __asm__ volatile("mv a1, %0" : : "r" ((uint64)handle));
    __asm__ volatile("mv a0, %0" : : "r" (0x22));

    __asm__ volatile("ecall");

    uint64 volatile ret;
    __asm__ volatile("mv %0, a0" : "=r" (ret));
    return (int)ret;
}

int sem_wait(sem_t id)
{
    __asm__ volatile("mv a1, %0" : : "r" ((uint64)id));
    __asm__ volatile("mv a0, %0" : : "r" (0x23));

    __asm__ volatile("ecall");

    uint64 volatile ret;
    __asm__ volatile("mv %0, a0" : "=r" (ret));
    return (int)ret;
    }

int sem_signal(sem_t id)
{
    __asm__ volatile("mv a1, %0" : : "r" ((uint64)id));
    __asm__ volatile("mv a0, %0" : : "r" (0x24));

    __asm__ volatile("ecall");

    uint64 volatile ret;
    __asm__ volatile("mv %0, a0" : "=r" (ret));
    return (int)ret;
    }

int sem_trywait(sem_t id)
{
    __asm__ volatile("mv a1, %0" : : "r" ((uint64)id));
    __asm__ volatile("mv a0, %0" : : "r" (0x26));

    __asm__ volatile("ecall");

    uint64 volatile ret;
    __asm__ volatile("mv %0, a0" : "=r" (ret));
    return (int)ret;
}


char getc()
{
    __asm__ volatile("mv a0, %0" : : "r" (0x41));

    __asm__ volatile("ecall");

    uint64 volatile ret;
    __asm__ volatile("mv %0, a0" : "=r" (ret));
    return (char)ret;
}

void putc(char c){
    __asm__ volatile("mv a1, %0" : : "r" ((uint64)c));
    __asm__ volatile("mv a0, %0" : : "r" (0x42));
    __asm__ volatile("ecall");
}


int sem_timedwait(sem_t id, time_t timeout){
    return 0;
}

int time_sleep(time_t){
    return 0;
}