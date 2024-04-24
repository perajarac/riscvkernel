#ifndef _queue_h_
#define _queue_h_
#include "../h/tcb.hpp"
#include "../h/memoryallocator.hpp"


class Queue{

public:

    struct TCBS{
        TCB* tcb;
        TCBS* next;
    };
    TCBS *head, *tail;
    
    Queue():head(0),tail(0){}


    void put(TCBS* t);
    TCB* pop();
    TCB* get(TCBS* t){return t->tcb;}
};








#endif