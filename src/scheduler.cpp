#include "../h/scheduler.hpp"


Queue Scheduler::q;

void Scheduler::put(TCB *tcb)
{
    tcb->setState(TCB::READY);
    q.put(Queue::TCBS(tcb));
}

TCB* Scheduler::get(){
    return q.pop();
}