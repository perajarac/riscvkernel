#ifndef _scheduler_hpp_
#define _scheduler_hpp_

class TCB;
class Queue;

class Scheduler{

public:
    static void put(TCB* tcb);
    static TCB* get();

private:
    static Queue q;

};

#endif