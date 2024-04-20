#include "../h/syscall_c.hpp"
#include "../h/syscall_cpp.hpp"
#include "../h/printf.hpp"
#include "../h/riscv.hpp"
#include "../h/memoryallocator.hpp"
void main()
{
    MemoryAllocator::initialize();
    RiscV::w_stvec((uint64) RiscV::handleTrap);
    MemoryAllocator::printFreeList();
    int *ret = new int[10];
    for(int i = 0; i < 10; i++)
    {
        *(ret + i) = i;
    }

    for(int i = 0; i < 10; i++)
    {
        printf("%d", (void*)(ret+i));
    }

    char* p = (char*)mem_alloc(10);

    
    mem_free(ret);
    mem_free(p);
    MemoryAllocator::printFreeList();
}
