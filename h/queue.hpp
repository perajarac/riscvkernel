#ifndef _queue_hpp_
#define _queue_hpp_
#include "../h/tcb.hpp"

class Queue{
public:

    struct TCBS{
        TCB* tcb;
        TCBS* next;

        TCBS(TCB* p):tcb(p),next(nullptr) {}

        void* operator new(size_t size){
            uint64 ssize = (size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
            return MemoryAllocator::kernel_mem_alloc(ssize);
        }
        void operator delete(void* p){MemoryAllocator::kernel_mem_free(p);}
    };  
    TCBS *head, *tail;
    
    Queue():head(0),tail(0){}


    void put(TCBS* t);
    TCB* pop();
    TCB* get(TCBS* t){return t->tcb;}

};

#endif