#include "../h/riscv.hpp"

void RiscV::handleTrap(uint64 op, uint64 a1, uint64 a2, uint64 a3,uint64 a4)
{
    volatile uint64 scause = RiscV::r_scause();

    if(scause & SCAUSE_INTTERUPT){
        return;
        //TODO: obrada spoljasnjeg prekidad
    }else{
        //unutrasnji
        if (scause == SCAUSE_IS || scause == SCAUSE_LAF || scause == SCAUSE_WAF) 
        { 
            TCB::syscall_thread_exit();
            return; 
        }

        if (scause == SCAUSE_USER || scause == SCAUSE_SYSTEM){
            uint64 volatile sepc = r_sepc() + 4;
            uint64 volatile sstatus = r_sstatus();


            switch(op){
                case(0x01): w_a0((uint64)MemoryAllocator::kernel_mem_alloc(a1)); break;
                case(0x02): w_a0((uint64)MemoryAllocator::kernel_mem_free((void *)a1)); break;
                case(0x11): TCB::syscall_thread_create(a1,a2,a3,a4);break;
                case(0x12): TCB::syscall_thread_exit();break;
                case(0x13): TCB::syscall_thread_dispatch();break;
                case(0x21): break;
                case(0x22): break;
                case(0x23): break;
                case(0x24): break;
                case(0x25): break;
                case(0x26): break;
                case(0x31): break;
                case(0x41): break;
                case(0x42): break;
                default: break;
            }

            w_sstatus(sstatus);
            w_sepc(sepc);            
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

void RiscV::setPrivilegeLevel() {
    if (TCB::running->status == TCB::Status::PRIVILEGED)
        ms_sstatus(SSTATUS_SPP);
    else
        mc_sstatus(SSTATUS_SPP);
}