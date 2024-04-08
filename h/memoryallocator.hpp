#ifndef _memoryallocator_hpp_
#define _memoryallocator_hpp_

#include "../lib/hw.h"
#include "../lib/console.h"

struct FreeMemory{
    size_t size;
    FreeMemory* next;
    FreeMemory* prev;
};

class MemoryAllocator{
    public:
        static void* kernel_mem_alloc(size_t sz);
        static int kernel_mem_free(void* adr);
        static FreeMemory* head;

        static void printFreeList();

    private:
        static bool isInitialized;
        static void tryToJoin(FreeMemory* cur);
        static void printFM(FreeMemory* head, int);

};

#endif