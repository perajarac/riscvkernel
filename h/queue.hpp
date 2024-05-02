#ifndef _queue_hpp_
#define _queue_hpp_
#include "../h/tcb.hpp"

class Queue{
public:

    static TCB *head, *tail;


    static void put(TCB* t);
    static TCB* pop();
    static TCB* front(){return head;}

    static void printQueue();

};

#endif