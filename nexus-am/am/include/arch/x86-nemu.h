#ifndef __ARCH_H__
#define __ARCH_H__

struct _Context {
  // uintptr_t eax, ecx, edx, ebx, esp, ebp, esi, edi, eflags, cs, eip;
  struct _AddressSpace *as;
  uintptr_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
  int irq;
  uintptr_t eip, cs, eflags;
  // uintptr_t esi, ebx, eax, eip, edx, eflags, ecx, cs, esp, edi, ebp;
  // int irq;
};

#define GPR1 irq
#define GPR2 eip
#define GPR3 eip
#define GPR4 eip
#define GPRx eip

#endif
