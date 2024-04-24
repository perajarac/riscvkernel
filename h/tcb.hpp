#ifndef _tcb_hpp_
#define _tcb_hpp_

#include "../h/memoryallocator.hpp"
#include "../h/syscall_c.hpp"

class TCB
{
    struct Context
    {
        uint64 sp;
        uint64 ra;
    };
    Context context;

    using Body = void (*)(void *);

    Body body;
    void *args;
    void *stack_space;

    const uint64 time_slice;
    static uint64 time_slice_counter;

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
    //TODO: static void timerInterrupt();

    static void join(TCB *thread);

    uint64 sleep_limit = 0;
    void setSleepThreshold(uint64 time) { sleep_limit = time; }

    void setState(State s) { state = s; }
    bool isFinished() {return state == FINISHED; }


    //switch context
    static void conswtch(TCB::Context *old, TCB::Context *neww);

    // memory allocation
    void* operator new(size_t size);
    void operator delete(void *ptr);

    // system call handlers
    static void sc_thread_create();
    static void sc_thread_exit();
    static void sc_thread_dispatch();

    // friends
    friend class RiscV;
};

#endif