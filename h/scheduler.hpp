#ifndef _scheduler_h_
#define _scheduler_h_

#include "../h/riscv.hpp"
#include "../h/tcb.hpp"
#include "../h/queue.hpp"



class Scheduler{

    static void put(TCB* tcb);
    static TCB* get();

    static Queue q;

};



#endif