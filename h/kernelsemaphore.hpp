#ifndef _kernel_semaphore_
#define _kernel_semaphore_
#include  "../h/tcb.hpp"

class KernelSemaphore{
private:

    int val = 0;
    void block();
    void unblock();

    struct SNode{
        TCB* blocked;
        SNode* next;

        void* operator new(size_t size){
            uint64 ssize = (size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
            return MemoryAllocator::kernel_mem_alloc(ssize);
        }
        void operator delete(void* ptr){
            MemoryAllocator::kernel_mem_free(ptr);
        }
    };

    SNode* head = nullptr;
    SNode* tail = nullptr;


public:

    KernelSemaphore(int val):val(val) {}
    ~KernelSemaphore();
    int getVal() const { return val; }
    
    int wait();
    void signal();
    int close();
    
    void* operator new(size_t size);
    void operator delete(void* ptr);


};






#endif