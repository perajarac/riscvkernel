#include "../h/syscall_c.hpp"

void* mem_alloc(size_t size) {
    uint64 ssize = (size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;

    __asm__ volatile("mv a1, %0" : : "r" (ssize));
    __asm__ volatile("mv a0, %0" : : "r" (0x01));
    __asm__ volatile("ecall");

    uint64 ret;
    __asm__ volatile("mv %0, a0" : "=r" (ret));
    return (void*)ret;
}

int mem_free(void* addr)
{
    __asm__ volatile("mv a1, %0" : : "r" ((uint64)addr));
    __asm__ volatile("mv a0, %0" : : "r" (0x02));
    __asm__ volatile("ecall");

    uint64 ret;
    __asm__ volatile("mv %0, a0" : "=r" (ret));
    return (int)ret;
}
int thread_create(thread_t* handle, void (*start_routine)(void*), void* arg) {
    void* stack_space = mem_alloc(DEFAULT_STACK_SIZE);
    if (!stack_space) return -1;

    __asm__ volatile("mv a4, %0" : : "r" ((uint64)stack_space));
    __asm__ volatile("mv a3, %0" : : "r" ((uint64)args));
    __asm__ volatile("mv a2, %0" : : "r" ((uint64)body));
    __asm__ volatile("mv a1, %0" : : "r" ((uint64)handle_ptr));
    __asm__ volatile("mv a0, %0" : : "r" (0x11));

    __asm__ volatile("ecall");

    uint64 ret;
    __asm__ volatile("mv %0, a0" : "=r" (ret));
    return (int)ret;
}