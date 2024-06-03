#include "../h/syscall_cpp.hpp"

void *operator new(size_t n)
{
    return mem_alloc(n);
}

void *operator new[](size_t n)
{
    return mem_alloc(n);
}

void operator delete(void *p)
{
    mem_free(p);
}

void operator delete[](void *p)
{
    mem_free(p);
}

Thread::Thread() : myHandle(nullptr), body(nullptr), arg(nullptr) {}
Thread::Thread(void (*body)(void *), void *arg):myHandle(nullptr), body(body), arg(arg){}

Thread::~Thread(){}

void Thread::wrapper(void *arg) {
    if(arg)((Thread*)arg)->run();
}

int Thread::start(){
    if(this->myHandle != nullptr) return -1;
    if(body == nullptr){
         return thread_create(&myHandle, Thread::wrapper, this);  
    }
    else {
        return thread_create(&myHandle, body, arg);
    }
}

void Thread::dispatch()
{
    thread_dispatch();
}

int Thread::sleep(time_t t){return 0;}

Semaphore::Semaphore(unsigned init)
{
    sem_open(&myHandle, init);
}

Semaphore::~Semaphore()
{
    sem_close(myHandle);
}

int Semaphore::wait()
{
    return sem_wait(myHandle);
}

int Semaphore::signal()
{
    return sem_signal(myHandle);
}

int Semaphore::timedWait(time_t t)
{
    return time_sleep(t);
}

int Semaphore::tryWait(){
    return sem_trywait(myHandle);
}

char Console::getc()
{
    return ::getc();
}

void Console::putc(char c)
{
    ::putc(c);
}
