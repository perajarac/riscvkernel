#include "../h/tcb.hpp"
#include "../h/syscall_c.hpp" 

TCB* TCB::running = nullptr;
uint64 TCB::time_slice_counter = 0;

TCB::TCB(Body body, void *args, void* stack_space, uint64 timeSlice, Status status):
        body(body), args(args),
        stack_space((char*)stack_space),
        time_slice(timeSlice),
        context({
            (uint64)(this->stack_space + DEFAULT_STACK_SIZE), //stack goes down to the beginning of the pointer
            (uint64)&set_runner
        }),
        status(status) {
            this->start();
        }

void TCB::start(){
    Scheduler::put(this);
}

void TCB::set_runner() {
    RiscV::popSppSpie();
    running->body(running->args);
    running->setState(State::FINISHED);
    dispatch();
}

void TCB::yield() {
    uint64 volatile sepc = RiscV::r_sepc();
    uint64 volatile sstatus = RiscV::r_sstatus();
    TCB::dispatch();
    RiscV::w_sstatus(sstatus);
    RiscV::w_sepc(sepc);
}

void TCB::dispatch() {

    TCB* old = TCB::running;
    if(old){
        if (!old->isFinished() && old->getState() != TCB::State::BLOCKED) {
            Scheduler::put(old);
        }
    }
    TCB::running = Scheduler::get();
    if(!running){
        running = old;
        return;
    } //if scheduler is empty return(nothing to switch with)


    time_slice_counter = 0;
    TCB::conswtch(&old->context, &running->context);
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

    *thread_handle = temp;

    if (temp) {
        RiscV::w_a0(0);
    } else {
        RiscV::w_a0(-1);
    }
}

void TCB::syscall_thread_exit(){
    if(!TCB::running){
        RiscV::w_a0(-1);
        return;
    }
    running->setState(State::FINISHED);
    yield();
    RiscV::w_a0(0);
}