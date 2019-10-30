#include "nemu.h"
#include "monitor/diff-test.h"

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
	if (ref_r->eax != cpu.eax) { 
		printf("cpu.eax wrong! Should be 0x%x!\n", ref_r->eax);
		return false;
	}
	if (ref_r->ecx != cpu.ecx) { 
		printf("cpu.ecx wrong! Should be 0x%x!\n", ref_r->ecx);
		return false;
	}
	if (ref_r->edx != cpu.edx) { 
		printf("cpu.edx wrong! Should be 0x%x!\n", ref_r->edx);
		return false;
	}
	if (ref_r->ebx != cpu.ebx) { 
		printf("cpu.ebx wrong! Should be 0x%x!\n", ref_r->ebx);
		return false;
	}
	if (ref_r->esp != cpu.esp) { 
		printf("cpu.esp wrong! Should be 0x%x!\n", ref_r->esp);
		return false;
	}
	if (ref_r->ebp != cpu.ebp) { 
		printf("cpu.ebp wrong! Should be 0x%x!\n", ref_r->ebp);
		return false;
	}
	if (ref_r->esi != cpu.esi) { 
		printf("cpu.esi wrong! Should be 0x%x!\n", ref_r->esi);
		return false;
	}
	if (ref_r->edi != cpu.edi) { 
		printf("cpu.edi wrong! Should be 0x%x!\n", ref_r->edi);
		return false;
	}
	if (ref_r->pc != cpu.pc) { 
		printf("cpu.pc wrong! Should be 0x%x!\n", ref_r->pc);
		printf("original pc is 0x%x.\n", pc);
		return false;
	}
	// if (ref_r->pc == pc) return false;
	// if (ref_r->eflags.CF != cpu.eflags.CF) return false;
	// if (ref_r->eflags.ZF != cpu.eflags.ZF) return false;
	// if (ref_r->eflags.SF != cpu.eflags.SF) return false;
	// if (ref_r->eflags.IF != cpu.eflags.IF) return false;
	// if (ref_r->eflags.OF != cpu.eflags.OF) return false;
	return true;
}

void isa_difftest_attach(void) {
}
