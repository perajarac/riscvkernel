#include "../h/scheduler.hpp"
#include "../h/queue.hpp"
#include "../h/tcb.hpp"


Scheduler &Scheduler::getInstance() {
    static Scheduler instance;
    return instance;
}

void Scheduler::put(TCB* tcb) {
    Scheduler& instance = Scheduler::getInstance();

    TCB_Node* temp = new TCB_Node(tcb);

    if(instance.head == nullptr) {
        instance.head = temp;
        instance.tail = temp;
    } else {
        instance.tail->next = temp;
        instance.tail = temp;
    }
}

TCB *Scheduler::get() {
    Scheduler& instance = Scheduler::getInstance();

    if(instance.head == nullptr)
        return nullptr;

    TCB_Node* retu = instance.head;
    instance.head = instance.head->next;

    if(instance.head == nullptr)
        instance.tail = nullptr;

    TCB* ret = retu->tcb;
    delete retu;

    return ret;
}
