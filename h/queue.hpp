#ifndef _queue_hpp_
#define _queue_hpp_
#include "../h/tcb.hpp"
#include "../h/memoryallocator.hpp"


class Queue{
public:

    struct TCBS{
        TCB* tcb;
        TCBS* next;

        TCBS(TCB* p):tcb(p),next(nullptr) {}
    };
    TCBS *head, *tail;
    
    Queue():head(0),tail(0){}


    void put(TCBS* t);
    TCB* pop();
    TCB* get(TCBS* t){return t->tcb;}

};

#endif