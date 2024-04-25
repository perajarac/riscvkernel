#include "../h/tcb.hpp" 

TCB* TCB::running = nullptr;
uint64 TCB::time_slice_counter = 0;

TCB::TCB(Body body, void *args, void* stack_space, uint64 timeSlice):
        body(body), args(args),
        stack_space(stack_space),
        time_slice(timeSlice),
        context({
            (uint64) ((uint8*)stack_space + DEFAULT_STACK_SIZE),
            (uint64) TCB::set_runner
        }) {}

void TCB::start(){
    Scheduler::put(this);
}

void TCB::set_runner() {
    RiscV::popSppSpie();
    running->body(running->args);
    syscall_thread_exit();
}

void TCB::dispatch() {
    TCB* old = running;
    if (old->state == State::RUNNABLE)
        Scheduler::put(old);
    running = Scheduler::get();
    running->setState(State::RUNNABLE);

    time_slice_counter = 0;
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

void TCB::syscall_thread_create(uint64 r1, uint64 r2, uint64 r3, uint64 r4)
{
    TCB** thread_handle = (TCB**)r1;
    Body start_routine = (Body)r2;
    void* args = (void*)r3;
    void* stack_space = (void*)r4;

    TCB *temp = new TCB(start_routine, args, stack_space);

    (*thread_handle) = temp;

    if (temp) {
        temp->start();
        RiscV::w_a0(0);
    } else {
        RiscV::w_a0(-1);
    }
}

void TCB::syscall_thread_dispatch()
{
    TCB *old = running;
    if (!old->isFinished()) { Scheduler::put(old); }
    running = Scheduler::get();

    TCB::conswtch(&old->context, &running->context);
}

void TCB::syscall_thread_exit(){
    running->setState(State::FINISHED);
    dispatch();
    RiscV::w_a0(0);
}