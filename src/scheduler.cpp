#include "../h/scheduler.hpp"

void Scheduler::put(TCB *tcb)
{
    tcb->setState(TCB::READY);
    q.put(tcb);
}

TCB* Scheduler::get(){
    return q.pop();
}