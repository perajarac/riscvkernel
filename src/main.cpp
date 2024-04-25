#include "../h/workers.hpp"

TCB* systemThread = nullptr;
TCB* userThread = nullptr;

void userMain(void* v){
    thread_t threads[2];
    thread_create(&threads[0], workerBodyA, nullptr);
    printf("ThreadA created\n");
    thread_create(&threads[1], workerBodyB, nullptr);
    printf("ThreadB created\n");
}

void main() {
    
    MemoryAllocator::initialize();
    RiscV::w_stvec((uint64) RiscV::handleTrap);

    systemThread = new TCB(0, 0, 0, 0);
    systemThread->status = TCB::Status::PRIVILEGED;
    TCB::running = systemThread;
    TCB::running->setState(TCB::State::RUNNABLE);
    userThread = new TCB(userMain, nullptr);
    userThread->status = TCB::Status::USER;
    userThread->start();
    while (!userThread->isFinished()) {
        thread_dispatch();
    }

    printf("gotovo");
}
