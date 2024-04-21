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