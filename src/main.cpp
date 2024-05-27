#include "../h/syscall_cpp.hpp"
#include "../test/printing.hpp"

extern void userMain();

void userMainWrapper(void* o) {
    userMain();
}


void main() {
    MemoryAllocator::initialize();
    RiscV::w_stvec((uint64) RiscV::supervisorTrap);
    thread_t user_main;
    thread_t main;


    thread_create(&main, nullptr, nullptr);
    main->setStatus(TCB::Status::PRIVILEGED);
    TCB::running = main;
    
    thread_create(&user_main, userMainWrapper, nullptr);
    user_main->setStatus(TCB::Status::USER);

    

    while(!user_main->isFinished()) 
    {
        thread_dispatch();
    }

}
