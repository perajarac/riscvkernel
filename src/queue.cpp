#include "../h/queue.hpp"

void Queue::put(TCB* t){
    t->next = nullptr;
    head = !head ? (tail = t) : (tail->next = t, tail = t, head);
}

TCB* Queue::pop(){
    if(!head) return nullptr;
    TCB* ret = head;
    head = head->next;
    if(!head)tail = nullptr;
    return ret;
}