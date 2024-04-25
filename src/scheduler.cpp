#include "../h/scheduler.hpp"
#include "../h/queue.hpp"
#include "../h/tcb.hpp"


Queue Scheduler::q;

void Scheduler::put(TCB *tcb)
{
    tcb->setState(TCB::State::READY);
    q.put(tcb);
}

TCB* Scheduler::get(){
    return q.pop();
}