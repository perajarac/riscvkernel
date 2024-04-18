#ifndef _tcb_hpp_
#define _tcb_hpp_

#pragma once

#include "MemoryAllocator.hpp"

class Thread
{

    using Body = void (*)(void *);

    static void initialize();
    Thread(Body body, void *args, void *stack_space = MemoryAllocator::kmalloc(DEFAULT_STACK_SIZE), uint64 timeSlice = DEFAULT_TIME_SLICE);
    ~Thread();

    static Thread *running;
    void start();
    static void runner();
    static void dispatch();
    static void timerInterrupt();

    static void join(Thread *thread);

    enum State
    {
        CREATED,
        READY,
        RUNNING, // runnable
        SUSPENDED,
        SLEEPING,
        FINISHED, // blocked
    };
    State state = CREATED;
    void setState(State s) { state = s; }
    bool finished() { return state == FINISHED; }

    uint64 sleepThreshold = 0;
    void setSleepThreshold(uint64 time) { sleepThreshold = time; }

    Body body;
    void *args;
    void *stack_space;

    const uint64 timeSlice;
    static uint64 timeSliceCounter;

    bool privileged = false;

    uint64 savedSP = 0;

    struct Context
    {
        uint64 sp;
        uint64 ra;
    };
    Context context;

    //switch context
    static void conswtch(Thread::Context *oldContext, Thread::Context *newContext);

    // memory allocation
    Thread* operator new(size_t size);
    Thread operator delete(void *ptr);

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