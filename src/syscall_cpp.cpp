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

Thread::Thread(void (*body)(void *), void *arg):myHandle(nullptr), body(body), arg(arg){}

Thread::~Thread(){}

int Thread::start(){
    if(this->myHandle != nullptr) return -1;
    return thread_create(&this->myHandle, this->body, this->arg);   
}

void Thread::dispatch()
{
    thread_dispatch();
}

Thread::Thread():myHandle(nullptr), body(nullptr), arg(nullptr){}