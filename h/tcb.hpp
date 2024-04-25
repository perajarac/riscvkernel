#ifndef _tcb_hpp_
#define _tcb_hpp_

#include "../h/riscv.hpp"
#include "../h/scheduler.hpp"

class TCB
{
public:
    struct Context
    {
        uint64 sp;
        uint64 ra;
    };

    using Body = void (*)(void *);

    Body body;
    void *args;
    void *stack_space;

    uint64 time_slice;
    static uint64 time_slice_counter;

    Context context;

    enum Status{
        USER,
        PRIVILEGED
    };
    Status status = PRIVILEGED; // Status

    

    TCB(Body body, void *args,
        void *stack_space = MemoryAllocator::kernel_mem_alloc((DEFAULT_STACK_SIZE + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE), 
        uint64 time_slice = DEFAULT_TIME_SLICE);
    ~TCB(){MemoryAllocator::kernel_mem_free(stack_space);}

    enum State
    {
        CREATED,
        READY,
        RUNNABLE, // runnable
        SUSPENDED,
        SLEEPING,
        FINISHED // blocked
    };
    State state = CREATED;

    static TCB *running;
    void start();
    static void set_runner();
    static void dispatch();
    //TODO: static void timer_interrupt();

    static void join(TCB *thread);

    uint64 sleep_limit = 0;
    void set_sleep_time(uint64 time) { sleep_limit = time; }

    void setState(State s) { state = s; }
    bool isFinished() {return state == FINISHED; }


    //switch context
    static void conswtch(TCB::Context *old, TCB::Context *neww);

    // memory allocation    
    void* operator new(size_t size);
    void operator delete(void *ptr);

    // system call handlers
    static void syscall_thread_create(uint64 r1, uint64 r2, uint64 r3, uint64 r4);
    static void syscall_thread_exit();
    static void syscall_thread_dispatch();

    friend class RiscV;
};

#endif