#include "../h/tcb.hpp" 

TCB* TCB::running = nullptr;;

TCB::TCB(Body body, void *args, void* stack_space, uint64 timeSlice):
        body(body), args(args),
        stack_space(stack_space),
        timeSlice(timeSlice),
        context({
            (uint64) ((uint8*)stack_space + DEFAULT_STACK_SIZE),
            (uint64) TCB::set_runner
        }) {}


void TCB::set_runner() {
    RiscV::popSppSpie();
    running->body(running->args);
    thread_exit();
}

void TCB::dispatch() {
    TCB* old = running;
    if (old->state == RUNNING)
        Scheduler::put(old);
    running = Scheduler::get();
    running->setState(RUNNING);

    timeSliceCounter = 0;
    RiscV::setPrivilegeLevel();
    conswtch(&old->context, &running->context);
}

void* TCB::operator new(size_t size){
    uint64 ssize = (size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    return MemoryAllocator::kernel_mem_alloc(ssize);
}

void TCB::operator delete(void* ptr){
    MemoryAllocator::kernel_mem_free(ptr);
}
