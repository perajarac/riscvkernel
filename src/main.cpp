#include "../lib/console.h"
#include "../lib/hw.h"
#include "../h/memoryallocator.hpp"
#include "../h/printf.hpp"
#include "../h/riscv.hpp"

void main()
{
    int *ret = (int *)MemoryAllocator::kernel_mem_alloc(10); // 10blokiova
    for (int i = 0; i < 10; i++)
    {
        *(ret + i) = i;
    }

    for (int i = 0; i < 10; i++)
    {
        printf("%d ", ret);
        ret++;
    }
}
