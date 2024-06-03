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

    enum Status{
        USER,
        PRIVILEGED
    };

    enum State
    {
        CREATED,
        BLOCKED, // blocked
        READY,
        RUNNABLE, // runnable
        FINISHED // blocked
    };

    static TCB *running;

    TCB(Body body, void *args,
        void *stack_space =MemoryAllocator::kernel_mem_alloc(BLOCKS_FOR_STACK_SIZE), 
        Status status = USER);
    ~TCB(){MemoryAllocator::kernel_mem_free(stack_space);}

    void start();
    static void setRunner();
    static void dispatch();
    static void yield();

    void setSemRet(int ret) {this->semRet = ret;}
    int getSemRet() const {return this->semRet;}

    void setState(State s) { state = s; }
    State getState() const { return state; }
    bool isFinished() {return state == FINISHED; }

    void setBody();
    void setArgs(void* args) { this->args = args; }

    void setStatus(Status status){this->status = status; }
    Status getStatus(){return this->status;}


    //switch context
    static void conswtch(TCB::Context *old, TCB::Context *neww);

    // memory allocation    
    void* operator new(size_t size);
    void operator delete(void *ptr);

    // system call handlers
    static void syscall_thread_create(uint64 r1, uint64 r2, uint64 r3, uint64 r4);
    static void syscall_thread_exit();

    friend class RiscV;
    /*------------------------------------------------------------------------*/
private:
    int semRet = 0;

    Body body;
    void *args;
    char *stack_space;

    Context context;

    Status status; // Status    

    State state = CREATED;


};

#endif