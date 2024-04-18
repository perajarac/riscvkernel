#include "../h/riscv.hpp"

void RiscV::handle_routine()
{
    uint64 scause = Riscv::r_scause();
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
            
        }
    }
}