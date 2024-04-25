#include "../h/workers.hpp"

TCB* systemThread = nullptr;
TCB* userThread = nullptr;

// void userMain(void*v){
//     thread_t threads[2];
//     thread_create(&threads[0], workerBodyA, nullptr);
//     printf("ThreadA created\n");
//     thread_create(&threads[1], workerBodyB, nullptr);
//     printf("ThreadB created\n");
// }

void main() {
    
    MemoryAllocator::initialize();
    RiscV::w_stvec((uint64) RiscV::handleTrap);
    //RiscV::mc_sstatus(SSTATUS_SPP);
    thread_t threads[2];
    thread_create(&threads[0], workerBodyA, nullptr);
    printf("ThreadA created\n");
    thread_create(&threads[1], workerBodyB, nullptr);
    printf("ThreadB created\n");

    TCB *thread1 = new TCB(workerBodyA, nullptr);
    thread1->start();
    
    TCB *thread2 = new TCB(workerBodyB, nullptr);
    thread2->start();
    // systemThread = new TCB(0, 0, 0, 0);
    // systemThread->status = TCB::Status::PRIVILEGED;
    // TCB::running = systemThread;
    // TCB::running->setState(TCB::State::RUNNABLE);
    // userThread = new TCB(userMain, nullptr);
    // userThread->status = TCB::Status::USER;
    // userThread->start();
    while (thread1->state == TCB::State::FINISHED && thread2->state == TCB::State::FINISHED) {
        thread_dispatch();
    }
    printf("gotovo");
}
