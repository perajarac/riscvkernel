#ifndef _scheduler_hpp_
#define _scheduler_hpp_

class TCB;
class Queue;

class Scheduler{

    static void put(TCB* tcb);
    static TCB* get();

    static Queue q;

};

#endif