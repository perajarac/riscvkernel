#ifndef riscv_h_
#define riscv_h_
#include "../lib/hw.h"
#include "../h/printf.hpp"
#include "../h/memoryallocator.hpp"
#include "../h/tcb.hpp"

#define SCAUSE_INTTERUPT (1L << 63) // Upper scause bit(decides if interrupt is outside or inside)
#define SSTATUS_SPP (1L << 8)       // Previous mode, 1=Supervisor, 0=User
#define SSTATUS_SPIE (1L << 5)      // Supervisor Previous Interrupt Enable
#define SSTATUS_UPIE (1L << 4)      // User Previous Interrupt Enable
#define SSTATUS_SIE (1L << 1)       // Supervisor Interrupt Enable
#define SSTATUS_UIE (1L << 0)       // User Interrupt Enable

// BNT = 0
#define SCAUSE_IS 2  // instruction fault
#define SCAUSE_LAF 5 // read access fault
#define SCAUSE_WAF 7 // store access fault
#define SCAUSE_USER 8
#define SCAUSE_SYSTEM 9

// BNT = 1
#define SCAUSE_TIMER 1
#define SCAUSE_CONSOLE 9

class RiscV
{
public:
  // defined in .cpp
  static void handleTrap(uint64 op, uint64 a1, uint64 a2, uint64 a3, uint64 a4);
  static void supervisorTrap();
  static void popSppSpie();
  static void setPrivilegeLevel();

  // inlines

  static uint64 r_sstatus();

  static void w_sstatus(uint64 status);

  static uint64 r_sip();

  static void w_sip(uint64 sip);

  static uint64 r_scause();

  static void w_scause(uint64 scause);

  static uint64 r_sepc();

  static void w_sepc(uint64 sepc);

  static uint64 r_stvec();

  static void w_stvec(uint64 stvec);

  static uint64 r_stval();

  static void w_stval(uint64 stval);

  static void ms_sstatus(uint64 mask);

  static void mc_sstatus(uint64 mask);

  static void ms_sip(uint64 mask);

  static void mc_sip(uint64 mask);

  static uint64 r_sscratch();

  static uint64 r_a0();

  static void w_a0(uint64 x);

  static uint64 r_a1();

  static void w_a1(uint64 x);

  static void inte(); // enabling intterupts
  static void intd(); // disabling intterupts

  friend class TCB;

};

inline void RiscV::inte()
{
  ms_sstatus(SSTATUS_SIE);
}

inline void RiscV::intd()
{
  mc_sstatus(SSTATUS_SIE);
}

inline uint64 RiscV::r_sstatus()
{
  uint64 volatile x;
  __asm__ volatile("csrr %0, sstatus" : "=r"(x));
  return x;
}

inline void RiscV::w_sstatus(uint64 status)
{
  __asm__ volatile("csrw sstatus, %0" : : "r"(status));
}

inline uint64 RiscV::r_sip()
{
  uint64 volatile x;
  asm volatile("csrr %0, sip" : "=r"(x));
  return x;
}

inline void RiscV::w_sip(uint64 sip)
{
  asm volatile("csrw sip, %0" : : "r"(sip));
}

inline uint64 RiscV::r_scause()
{
  uint64 volatile x;
  __asm__ volatile("csrr %0, scause" : "=r"(x));
  return x;
}

inline void RiscV::w_scause(uint64 scause)
{
  __asm__ volatile("csrw scause, %0" : : "r"(scause));
}

inline uint64 RiscV::r_sepc()
{
  uint64 volatile x;
  __asm__ volatile("csrr %0, sepc" : "=r"(x));
  return x;
}

inline void RiscV::w_sepc(uint64 sepc)
{
  __asm__ volatile("csrw sepc, %0" : : "r"(sepc));
}

inline uint64 RiscV::r_stvec()
{
  uint64 volatile x;
  __asm__ volatile("csrr %0, stvec" : "=r"(x));
  return x;
}

inline void RiscV::w_stvec(uint64 stvec)
{
  __asm__ volatile("csrw stvec, %0" : : "r"(stvec));
}

inline uint64 RiscV::r_stval()
{
  uint64 volatile x;
  __asm__ volatile("csrr %0, stval" : "=r"(x));
  return x;
}

inline void RiscV::w_stval(uint64 stval)
{
  __asm__ volatile("csrw stval, %0" : : "r"(stval));
}

inline void RiscV::ms_sstatus(uint64 mask)
{
  __asm__ volatile("csrs sstatus, %0" : : "r"(mask));
}

inline void RiscV::mc_sstatus(uint64 mask)
{
  __asm__ volatile("csrc sstatus, %0" : : "r"(mask));
}

inline void RiscV::ms_sip(uint64 mask)
{
  __asm__ volatile("csrs sip, %0" : : "r"(mask));
}

inline void RiscV::mc_sip(uint64 mask)
{
  __asm__ volatile("csrc sip, %0" : : "r"(mask));
}

inline uint64 RiscV::r_sscratch()
{
  uint64 volatile x;
  __asm__ volatile("csrr %0, sscratch" : "=r"(x));
  return x;
}

inline uint64 RiscV::r_a0()
{
  uint64 volatile x;
  __asm__ volatile("mv %0, a0" : "=r"(x));
  return x;
}

inline void RiscV::w_a0(uint64 x)
{
  __asm__ volatile("mv a0, %0" : : "r"(x));
}

inline uint64 RiscV::r_a1()
{
  uint64 volatile x;
  __asm__ volatile("mv %0, a1" : "=r"(x));
  return x;
}

inline void RiscV::w_a1(uint64 x)
{
  __asm__ volatile("mv a1, %0" : : "r"(x));
}

#endif