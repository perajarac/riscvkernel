#ifndef _tcb_hpp_
#define _tcb_hpp_

#include "../h/memoryallocator.hpp"

class TCB
{
    TCB(Body body, void *args, void *stack_space = MemoryAllocator::kernel_mem_alloc(DEFAULT_STACK_SIZE), uint64 time_slice = DEFAULT_TIME_SLICE);
    ~TCB();

    enum State
    {
        CREATED,
        READY,
        RUNNABLE, // runnable
        SUSPENDED,
        SLEEPING,
        FINISHED, // blocked
    };
    State state = CREATED;

    using Body = void (*)(void *);

    static TCB *running;
    void start();
    static void runner();
    static void dispatch();
    //TODO: static void timerInterrupt();

    static void join(TCB *thread);

    uint64 sleep_limit = 0;
    void setSleepThreshold(uint64 time) { sleepLimit = time; }

    void setState(State s) { state = s; }
    bool isFinished() {return state == FINISHED; }

    Body body;
    void *args;
    void *stack_space;

    const uint64 time_slice;
    static uint64 time_slice_counter;

    bool privileged = false;

    uint64 savedSP = 0;

    struct Context
    {
        uint64 sp;
        uint64 ra;
    };
    Context context;

    //switch context
    static void conswtch(TCB::Context *old, TCB::Context *new);

    // memory allocation
    TCB* operator new(size_t size);
    TCB operator delete(void *ptr);

    // system call handlers
    static void sc_thread_create();
    static void sc_thread_exit();
    static void sc_thread_dispatch();
    static void sc_thread_join();
    static void sc_thread_build();
    static void sc_thread_start();
    static void sc_thread_delete();
    static void sc_time_sleep();

    // friends
    friend class RiscV;
};

#endif