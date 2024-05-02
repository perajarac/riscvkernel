#include "../h/tcb.hpp" 

TCB* TCB::running = nullptr;
uint64 TCB::time_slice_counter = 0;

TCB::TCB(Body body, void *args, char* stack_space, uint64 timeSlice, Status status):
        body(body), args(args),
        stack_space(stack_space),
        time_slice(timeSlice),
        context({
            (uint64) ((char*)stack_space + DEFAULT_STACK_SIZE), //stack goes down to the beginning of the pointer
            (uint64) TCB::set_runner
        }),
        status(status) {
                Scheduler::put(this);
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

void TCB::dispatch() {

    TCB* old = TCB::running;
    if(old){
        if (!old->isFinished())
            Scheduler::put(old);
    }
    if(old == Scheduler::getHead()){
        Scheduler::bugg(); //fixing bug that old and geted thread are same beacause lack of threads
    }
    TCB::running = Scheduler::get();
    printf("%l ", old);
    printf("%l \n", running);


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
    char* stack_space = (char*)r4;

    TCB *temp = new TCB(start_routine, args, stack_space);

    *thread_handle = temp;

    if (temp) {
        if(!running)
            running = temp;
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
    dispatch();
    RiscV::w_a0(0);
}