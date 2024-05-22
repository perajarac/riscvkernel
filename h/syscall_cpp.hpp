#ifndef _syscall_cpp_h_
#define _syscall_cpp_h_
#include "../h/syscall_c.hpp"

void* operator new(size_t size);
void operator delete(void* addr);

void* operator new[](size_t size);
void operator delete[](void* addr);

class Thread {
public:
    Thread (void (*body)(void*), void* arg);
    virtual ~Thread ();
    int start ();
    static void dispatch ();
    static int sleep (time_t);
protected:
    Thread ();
    virtual void run () {}
private:
    thread_t myHandle;
    void (*body)(void*); void* arg;
};

class Semaphore {
public:
    Semaphore(unsigned init = 1);
    virtual ~Semaphore ();
    int wait ();
    int signal ();
    int timedWait(time_t);
    int tryWait();
private:
 sem_t myHandle;
};


//----------------------TODO----------------------------
class Console {
public:
 static char getc ();
 static void putc (char);
};



#endif