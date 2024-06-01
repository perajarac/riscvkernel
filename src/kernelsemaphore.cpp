#include "../h/kernelsemaphore.hpp"

void KernelSemaphore::block(){
    SNode* temp = new SNode();
    
    temp->next = nullptr;
    temp->blocked = TCB::running;

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

int KernelSemaphore::close(KernelSemaphore* ks){
    while(ks->head){
        ks->head->blocked->sem_ret = -1;
        ks->unblock();
    }
    return 0;
}

void* KernelSemaphore::operator new(size_t size){
    uint64 ssize = (size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    return MemoryAllocator::kernel_mem_alloc(ssize);
}

void KernelSemaphore::operator delete(void* ptr){
    MemoryAllocator::kernel_mem_free(ptr);
}

int KernelSemaphore::kernel_sem_open(uint64 handl, uint64 init)
{
    KernelSemaphore** handle = (KernelSemaphore **)handl;
    (*handle) = new KernelSemaphore((unsigned)init);
    if(*handle){
        return 0;
    }else{
        return -1;
    }
}

int KernelSemaphore::kernel_sem_close(uint64 handl)
{
    KernelSemaphore* handle = (KernelSemaphore*)handl;
    if(handle == nullptr) return -1;
    int ret = KernelSemaphore::close(handle);
    delete handle;
    return ret;
}

int KernelSemaphore::kernel_sem_wait(uint64 i)
{
    KernelSemaphore* id = (KernelSemaphore*)i;
    return id->wait();
    
}

int KernelSemaphore::kernel_sem_signal(uint64 i)
{
    KernelSemaphore* id = (KernelSemaphore*)i;
    if(id == nullptr) return -1;
    id->signal();
    return 0;
}

int KernelSemaphore::kernel_sem_trywait(uint64 i)
{
    KernelSemaphore* id = (KernelSemaphore*)i;
    if(id == nullptr) return -1;
    if(id->getVal() - 1 < 0) return 0;
    else return 1;
}
