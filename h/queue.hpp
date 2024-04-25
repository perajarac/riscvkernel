#ifndef _queue_hpp_
#define _queue_hpp_
#include "../h/tcb.hpp"

class Queue{
public:

    TCB *head, *tail;

    Queue():head(0),tail(0){}


    void put(TCB* t);
    TCB* pop();
    TCB* front(){return head;}

};

#endif