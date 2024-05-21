#include "../h/kernelsemaphore.hpp"

void KernelSemaphore::block(){
    SNode* temp = new SNode();
    temp->blocked = TCB::running;
    temp->next = nullptr;

    if(head == nullptr) {
        head = temp;
        tail = temp;
    } else {
        tail->next = temp;
        tail = temp;
    }

    TCB::running->setState(TCB::State::BLOCKED);

    TCB::yield();
}

void KernelSemaphore::unblock(){
    if(!head) return;

    SNode* to_unblock = head;
    head = head->next;
    if(!head) tail = nullptr;
    to_unblock->blocked->setState(TCB::State::RUNNABLE);
    Scheduler::put(to_unblock->blocked);
    delete to_unblock;
}

int KernelSemaphore::wait(){
    TCB::running->sem_ret = 0;
    if(--val<0)this->block();
    return TCB::running->sem_ret;
}

void KernelSemaphore::signal(){
    if(++val <= 0) unblock();
}

int KernelSemaphore::close(){
    while(this->head){
        this->head->blocked->sem_ret = -1;
        unblock();
    }
    return 0;
}

KernelSemaphore::~KernelSemaphore(){
    this->close();
}

void* KernelSemaphore::operator new(size_t size){
    uint64 ssize = (size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    return MemoryAllocator::kernel_mem_alloc(ssize);
}

void KernelSemaphore::operator delete(void* ptr){
    MemoryAllocator::kernel_mem_free(ptr);
}

