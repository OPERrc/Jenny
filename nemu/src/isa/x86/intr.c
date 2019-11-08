#include "rtl/rtl.h"

void raise_intr(uint32_t NO, vaddr_t ret_addr) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */
  rtl_push(&cpu.eflags);
  rtl_push(&cpu.cs);
  rtl_push(&cpu.pc);
  Log("0x%x\n", NO);
  TODO();
  cpu.pc = vaddr_read(NO, 4);
}

bool isa_query_intr(void) {
  return false;
}
