#ifndef _memoryallocator_hpp_
#define _memoryallocator_hpp_

#include "../lib/hw.h"
#include "../lib/console.h"
#include "../h/riscv.hpp"
#include "../h/printf.hpp"

struct FreeMemory
{
    size_t size;
    FreeMemory *next;
    FreeMemory *prev;
};

class MemoryAllocator
{
public:
    static void initialize();  //initialization of structures and pointers

    static void *kernel_mem_alloc(size_t sz);
    static int kernel_mem_free(void *adr);
    static FreeMemory *head;

    static void syscall_kmalloc();
    static void syscall_kfree();

    static void printFreeList();
    static void printFHead();

private:
    static void tryToJoin(FreeMemory *cur);
    static void printFM(FreeMemory *head, int);
};

inline void MemoryAllocator::printFHead(){
    printf("%l", (void*)head);
}

#endif