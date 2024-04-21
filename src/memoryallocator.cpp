#include "../h/memoryallocator.hpp"

FreeMemory *MemoryAllocator::head = nullptr;

void MemoryAllocator::initialize(){
    head = (FreeMemory *)((size_t)HEAP_START_ADDR);
    head->size = ((size_t)((size_t *)HEAP_END_ADDR - (size_t *)HEAP_START_ADDR)) / MEM_BLOCK_SIZE;
    head->next = nullptr;
    head->prev = nullptr;
}

void *MemoryAllocator::kernel_mem_alloc(size_t sz)
{
    void *ret = nullptr;

    for (FreeMemory *temp = head; temp; temp = temp->next)
    {
        if (temp->size >= sz + 1)
        {
            ret = temp;
            if (temp->size - (sz + 1) == 0)
            {
                if (temp->prev != nullptr)
                    temp->prev->next = temp->next;
                else
                    head = temp->next;
                if (temp->next != nullptr)
                    temp->next->prev = temp->prev;
                temp->size = sz + 1;
                break;
            }
            FreeMemory *leftOver = (FreeMemory *)((size_t)temp + (sz + 1) * MEM_BLOCK_SIZE);
            if (temp->prev)
                temp->prev->next = leftOver;
            else
                head = leftOver;
            if (temp->next)
                temp->next->prev = leftOver;
            leftOver->prev = temp->prev;
            leftOver->next = temp->next;
            leftOver->size = temp->size - (sz + 1);
            temp->size = sz + 1;
            break;
        }
    }

    return reinterpret_cast<void *>(reinterpret_cast<size_t>(ret) + MEM_BLOCK_SIZE);
}

uint32 MemoryAllocator::kernel_mem_free(void *adr)
{
    if ((size_t *)adr > (size_t *)HEAP_END_ADDR || (size_t *)adr < (size_t *)HEAP_START_ADDR)
        return 3; //ERROR

    adr = reinterpret_cast<void *>(reinterpret_cast<size_t>(adr) - MEM_BLOCK_SIZE);

    FreeMemory *prev = 0;
    if (head == nullptr || (size_t *)adr < (size_t *)head)
        prev = 0;
    else
    {
        for (prev = head; prev->next && (size_t *)adr > (size_t *)(prev->next); prev = prev->next)
            ;
    }
    FreeMemory *seg = (FreeMemory *)((size_t *)adr);
    seg->prev = prev;
    if (prev)
        seg->next = prev->next;
    else
    {
        seg->next = head;
    }
    if (seg->next)
        seg->next->prev = seg;
    if (prev)
        prev->next = seg;
    else
        head = seg;
    if (seg != nullptr)
        tryToJoin(seg);
    if (prev != nullptr)
        tryToJoin(prev);
    return 0;
}

void MemoryAllocator::tryToJoin(FreeMemory *cur)
{

    if (cur->next && (size_t)(cur->next) == ((size_t)cur + cur->size * MEM_BLOCK_SIZE))
    {
        cur->size += cur->next->size;
        cur->next = cur->next->next;
        if (cur->next)
            cur->next->prev = cur;
    }
}

void MemoryAllocator::printFreeList()
{
    FreeMemory *temp = head;
    int a = 0;
    while (temp)
    {
        if (temp == head)
            printFM(temp, 1);
        else
        {
            __putc('\n');
            __putc('0' + a);
            __putc(58);
            printFM(temp, 0);
        }
        temp = temp->next;
        a++;
    }
}
void MemoryAllocator::printFM(FreeMemory *head, int a)
{
    uint64 n = (uint64)head;
    int i = 0;
    if (a == 1)
    {
        __putc(104);
        __putc(101);
        __putc(97);
        __putc(100);
        __putc(58);
        a--;
    }

    char buffer[21];
    while (n > 0)
    {
        buffer[i++] = '0' + (n % 10);
        n /= 10;
    }

    while (i > 0)
    {
        __putc(buffer[--i]);
    }
}
