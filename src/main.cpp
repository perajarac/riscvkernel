#include "../h/syscall_c.hpp"
#include "../h/printf.hpp"
#include "../h/riscv.hpp"
#include "../h/memoryallocator.hpp"
void main()
{
    MemoryAllocator::initialize();
    RiscV::w_stvec((uint64) RiscV::handleTrap);
    MemoryAllocator::printFreeList();
    int *ret = (int *)mem_alloc(10);// 10blokiova
    for(int i = 0; i < 10; i++)
    {
        *(ret + i) = i;
    }

    
    mem_free((void*)ret);
    MemoryAllocator::printFreeList();
}
