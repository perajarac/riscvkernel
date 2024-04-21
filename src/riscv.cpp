#include "../h/riscv.hpp"

void RiscV::handleTrap()
{
    uint64 scause = RiscV::r_scause();
    bool outscause = scause & SCAUSE_INTTERUPT;
    scause &= ~SCAUSE_INTTERUPT;

    if(outscause){
        return;
        //TODO: obrada spoljasnjeg prekidad
    }else{
        //unutrasnji
        if (scause == SCAUSE_IS || scause == SCAUSE_LAF || scause == SCAUSE_WAF) 
        {
            return; //TODO:thread_exit;
        }

        if (scause == SCAUSE_USER || scause == SCAUSE_SYSTEM){
            uint64 volatile op_code = r_a0();
            uint64 volatile arg1 = r_a1();
            uint64 volatile sepc = r_sepc() + 4;
            uint64 volatile sstatus = r_sstatus();


            switch(op_code){
                case(0x01):MemoryAllocator::syscall_kmalloc(arg1); break;
                case(0x02):MemoryAllocator::syscall_kfree(arg1); break;
                case(0x11):break;
                case(0x12):break;
                case(0x13):break;
                case(0x21):break;
                case(0x22):break;
                case(0x23):break;
                case(0x24):break;
                case(0x25):break;
                case(0x26):break;
                case(0x31):break;
                case(0x41):break;
                case(0x42): break;
                default: break;
            }

            w_sstatus(sstatus);
            w_sepc(sepc);
            
            __asm__ volatile("nop");
            
        }
    }


}