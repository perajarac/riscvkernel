#include "../h/riscv.hpp"

void RiscV::handleTrap(uint64 op, uint64 a1, uint64 a2, uint64 a3,uint64 a4)
{
    uint64 volatile scause = RiscV::r_scause();

        
    if (scause == 0x8000000000000001UL){
        mc_sip(1<<1);
        return;
    }else if (scause == 0x8000000000000009UL){
        console_handler();
        return;
    }
        
        //TODO: obrada spoljasnjeg prekidad
    if (scause == 0x0000000000000008UL || scause == 0x0000000000000009UL){
        //unutrasnji
        if (scause == SCAUSE_IS || scause == SCAUSE_LAF || scause == SCAUSE_WAF) 
        { 
            printf("INSTRUCTION FAULT %d",(void*)r_stval());
            TCB::syscall_thread_exit();
        }

        if (scause == SCAUSE_USER || scause == SCAUSE_SYSTEM){
            uint64 volatile sepc = r_sepc() + 4;
            uint64 volatile sstatus = r_sstatus();
            RiscV::w_sepc(sepc);

            switch(op){
                case(0x01): w_a0((uint64)MemoryAllocator::kernel_mem_alloc(a1)); break;
                case(0x02): w_a0((uint64)MemoryAllocator::kernel_mem_free((void *)a1)); break;
                case(0x11): TCB::syscall_thread_create(a1,a2,a3,a4);break;
                case(0x12): TCB::syscall_thread_exit();break;
                case(0x13): TCB::dispatch(); break;
                case(0x21): w_a0(KernelSemaphore::kernel_sem_open(a1,a2)); break;
                case(0x22): w_a0(KernelSemaphore::kernel_sem_close(a1)); break;
                case(0x23): w_a0(KernelSemaphore::kernel_sem_wait(a1));break;
                case(0x24): w_a0(KernelSemaphore::kernel_sem_signal(a1)); break;
                case(0x25): break;
                case(0x26): w_a0(KernelSemaphore::kernel_sem_trywait(a1)); break;
                case(0x31): break;
                case(0x41): w_a0(__getc()); break;
                case(0x42): __putc(a1); break;
                default: break;
            }   

            RiscV::w_sstatus(sstatus);
        }
    }
}

void RiscV::popSppSpie(){
    if(TCB::running->status == TCB::Status::USER){
        RiscV::mc_sstatus(SSTATUS_SPP);
    }else{
        RiscV::ms_sstatus(SSTATUS_SPP);
    }
    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile("sret");
}

//If the stattus was privileged it sets the status back, otherwise it sets the status in user mode
void RiscV::setPrivilegeLevel() {
    if (TCB::running->status == TCB::Status::PRIVILEGED)
        ms_sstatus(SSTATUS_SPP);
    else
        mc_sstatus(SSTATUS_SPP);
}