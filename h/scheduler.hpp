#ifndef _scheduler_hpp_
#define _scheduler_hpp_

class TCB;
#include "../h/memoryallocator.hpp"
class Scheduler{

public:
    static void put(TCB* tcb);
    static TCB* get();

private:
    struct TCB_Node{
        TCB* tcb;
        TCB_Node* next;

        TCB_Node(TCB* tcb){
            this->tcb = tcb;
            this->next = nullptr;
        }
        void* operator new(size_t size){
            uint64 ssize = (size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
            return MemoryAllocator::kernel_mem_alloc(ssize);
        }
        void operator delete(void* ptr){
            MemoryAllocator::kernel_mem_free(ptr);
        }
    };

    TCB_Node* head = nullptr;
    TCB_Node* tail = nullptr;

    Scheduler(Scheduler&) = delete;
    Scheduler(Scheduler&&) = delete;
    Scheduler() {}

    static Scheduler& getInstance();

};

#endif