#include "../h/queue.hpp"

TCB* Queue::head = nullptr;
TCB* Queue::tail = nullptr;

void Queue::put(TCB* t){
    t->next = nullptr;
   if (tail)
    {
        tail->next = t;
        tail = t;
    }else
    {
        head = tail = t;
    }
}

TCB* Queue::pop(){
    if(!head) return nullptr;
    TCB* ret = head;
    head = head->next;
    if(!head)tail = head = nullptr;
    return ret;
}

void Queue::printQueue(){

    TCB* t = Queue::head;
    while(t)
    {
        uint64 a = (uint64)t;
        printf("%l " , &a);
        t = t->next;
    }
    
}
