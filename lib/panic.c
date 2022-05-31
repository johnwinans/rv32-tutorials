/*
*    RISC-V rv32 tutorial examples
*
*    Copyright (C) 2021 John Winans
*
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*
*/


#include "panic.h"
#include "hexdump.h"
#include "clint.h"
#include "csr.h"

#include <stdio.h>



// XXX these global areas are not viable when running with multiple cores!
// XXX need per-hart save areas for this stuff!

/**
* A place to store all the registers in a known-pattern for dumping.
***************************************************************************/
static uint32_t saved_xregs[32] __attribute__((used));
static uint32_t saved_mstatus __attribute__((used));
//static double saved_fregs[32] __attribute__((used));

static uint32_t panic_stack[1024*16] 
	__attribute__((used)) 			// Do not optimize this away
	__attribute__ ((aligned (16)))	// The ABI requires that the stack be aligned to 16 bytes!!
;


static char mcause_irq[16][32] = {
	"Reserved",
	"Supervisor software interrupt",
	"Reserved",
	"Machine software interrupt",
	"Reserved",
	"Supervisor timer interrupt",
	"Reserved",
	"Machine timer interrupt",
	"Reserved",
	"Supervisor external interrupt",
	"Reserved",
	"Machine external interrupt",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved"
};


static char mcause_trap[16][32] = {
	"Instruction address misaligned",
	"Instruction access fault",
	"Illegal instruction",
	"Breakpoint",
	"Load address misaligned",
	"Load access fault",
	"Store/AMO address misaligned",
	"Store/AMO access fault",
	"Environment call from U-mode",
	"Environment call from S-mode",
	"Reserved",
	"Environment call from M-mode",
	"Instruction page fault",
	"Load page fault",
	"Reserved",
	"Store/AMO page fault"
};

/**
* Print the misa register.
***************************************************************************/
int dump_misa(uint32_t misa)
{
	int len = 0;
    len += printf("rv32");
    for (uint8_t i=0; i<26; ++i)
    {
        if (misa&1)
            len += printf("%c", 'a'+i);
        misa>>=1;
    }
	return len;
}

/**
* Print the satp register.
***************************************************************************/
int dump_satp(uint32_t satp)
{
	int len = 0;
	if (satp & 0x80000000)
		len += printf("MODE=Sv32 ");
	else
		len += printf("MODE=Bare ");
	len += printf("ASID=%03lx ", (satp>>22)&0x1ff);
	len += printf("Proot=%08lx ", (satp&0x3fffff)<<12);
	return len;
}

/**
* Print the fields [ms]status register.
***************************************************************************/
int dump_status(uint32_t status)
{
	int len = 0;
	if (status & 0x80000000)
		len += printf("SD ");
	if (status & (1<<22))
		len += printf("TSR ");
	if (status & (1<<21))
		len += printf("TW ");
	if (status & (1<<20))
		len += printf("TVM ");
	if (status & (1<<19))
		len += printf("MXR ");
	if (status & (1<<18))
		len += printf("SUM ");
	if (status & (1<<17))
		len += printf("MPRV ");
	len += printf("XS=%lx ", (status>>15)&0x03);
	len += printf("FS=%lx ", (status>>13)&0x03);
	len += printf("MPP=%lx ", (status>>11)&0x03);
	if (status & (1<<8))
		len += printf("SPP ");
	if (status & (1<<7))
		len += printf("MPIE ");
	if (status & (1<<6))
		len += printf("UBE ");
	if (status & (1<<5))
		len += printf("SPIE ");
	if (status & (1<<3))
		len += printf("MIE ");
	if (status & (1<<1))
		len += printf("SIE ");
	return len;
}


int dump_tcause(uint32_t cause)
{
	int len = 0;
	if (cause&0x80000000)
	{
		cause &= 0x7fffffff;
		len += printf("IRQ %2lu: ", cause);
		if (cause > 15)
			len += printf("%s", "Designated for platform use");
		else
			len += printf("%s", mcause_irq[cause]);
	}
	else
	{
		len += printf("TRAP %2lu: ", cause);
		if (cause > 15)
			len += printf("%s", "Reserved/Designated for custom use");
		else
			len += printf("%s", mcause_trap[cause]);
	}
	return len;
}


/**
* If you fail to plan, you are planning to fail!  - B. Franklin
*
* @bug Properly implemented, this code would run out of ROM using 
* hard-coded/dedicated buffer areas for formatting & printing to the
* console... that would also be re-initialized from scratch (because
* all memory, devices and configurations could be corrupted upon entry 
* here.)
*
* @bug This code will only work in Machine mode.
***************************************************************************/
void panic_dump(const char *msg)
{
	uint32_t mhartid = csr_read_mhartid();

	char trailer[32];
	sprintf(trailer, " [%lu]\n", mhartid);


	printf("[%lu] panic: **** BEGIN *****************************************************************%s", mhartid, trailer);
	if (msg)
		printf("[%lu] panic: %-75s %s", mhartid, msg, trailer);

	uint64_t t = clint_get_mtime();

#if 1
	// This will spin on broken memory accesses if the stack is not aligned %8!!
	printf("[%ld] panic: mtime = 0x%16.16llx %54s", mhartid, t, trailer);
#else
	//printf("[%ld] panic: mtime = 0x%8.8lx%8.8lx %54s", mhartid, (uint32_t)(t>>32), (uint32_t)(t&0x0ffffffff), trailer);
#endif


    for (int i=0; i<32; ++i)
    {
        if (i%8 == 0)
			printf("%s[%lu] panic: %sx%d", (i!=0?trailer:""), mhartid, (i<10?" ":""), i);
        else if (i%4 == 0)
			printf(" ");
		printf(" %8.8x", (unsigned int)saved_xregs[i]);
    }
	printf("%s", trailer);



#if 0
#if 0
	printf("[%ld] sstatus = %8.8lx\n", mhartid, csr_read_sstatus());
	printf("[%ld] sie = %8.8lx\n", mhartid, csr_read_sie());
	printf("[%ld] stvec = %8.8lx\n", mhartid, csr_read_stvec());
	printf("[%ld] scounteren = %8.8lx\n", mhartid, csr_read_scounteren());
	printf("[%ld] sscratch = %8.8lx\n", mhartid, csr_read_sscratch());
	printf("[%ld] sepc = %8.8lx\n", mhartid, csr_read_sepc());
	printf("[%ld] scause = %8.8lx\n", mhartid, csr_read_scause());
	printf("[%ld] stval = %8.8lx\n", mhartid, csr_read_stval());
	printf("[%ld] sip = %8.8lx\n", mhartid, csr_read_sip());

	printf("[%ld] satp = %8.8lx\n", mhartid, csr_read_satp());
	//printf("[%ld] scontext = %8.8lx\n", mhartid, csr_read_scontext());

	printf("[%ld] mstatus    = %8.8lx\n", mhartid, csr_read_mstatus());
	printf("[%ld] mie        = %8.8lx\n", mhartid, csr_read_mie());
	printf("[%ld] mip        = %8.8lx\n", mhartid, csr_read_mip());
	printf("[%ld] misa       = %8.8lx\n", mhartid, csr_read_misa());
	printf("[%ld] mepc       = %8.8lx\n", mhartid, csr_read_mepc());
	printf("[%ld] mcause     = %8.8lx\n", mhartid, csr_read_mcause());
	printf("[%ld] mtval      = %8.8lx\n", mhartid, csr_read_mtval());
	printf("[%ld] medeleg    = %8.8lx\n", mhartid, csr_read_medeleg());
	printf("[%ld] mideleg    = %8.8lx\n", mhartid, csr_read_mideleg());
	printf("[%ld] mtvec      = %8.8lx\n", mhartid, csr_read_mtvec());
	printf("[%ld] mscratch   = %8.8lx\n", mhartid, csr_read_mscratch());

#else
	// spaced so it does not visually bleed into the register dump
	printf("[%ld] panic:    mstatus  = %8.8lx    sstatus  = %8.8lx %35s", mhartid, csr_read_mstatus(), csr_read_sstatus(), trailer);
	printf("[%ld] panic:    mie      = %8.8lx    sie      = %8.8lx %35s", mhartid, csr_read_mie(), csr_read_sie(), trailer);
	printf("[%ld] panic:    mip      = %8.8lx    sip      = %8.8lx %35s", mhartid, csr_read_mip(), csr_read_sip(), trailer);
	printf("[%ld] panic:    mepc     = %8.8lx    sepc     = %8.8lx %35s", mhartid, csr_read_mepc(), csr_read_sepc(), trailer);
	printf("[%ld] panic:    mcause   = %8.8lx    scause   = %8.8lx %35s", mhartid, csr_read_mcause(), csr_read_scause(), trailer);
	printf("[%ld] panic:    mtval    = %8.8lx    stval    = %8.8lx %35s", mhartid, csr_read_mtval(), csr_read_stval(), trailer);
	printf("[%ld] panic:    mtvec    = %8.8lx    stvec    = %8.8lx %35s", mhartid, csr_read_mtvec(), csr_read_stvec(), trailer);
	printf("[%ld] panic:    mscratch = %8.8lx    sscratch = %8.8lx %35s", mhartid, csr_read_mscratch(), csr_read_sscratch(), trailer);
#endif
#endif


	int len;

	printf("[%ld] panic:    mstatus  = %8.8lx ", mhartid, csr_read_mstatus());
	len = dump_status(csr_read_mstatus());
	printf("%*s", 58-len, trailer);
	printf("[%ld] panic:    mie      = %8.8lx %58s", mhartid, csr_read_mie(), trailer);
	printf("[%ld] panic:    mip      = %8.8lx %58s", mhartid, csr_read_mip(), trailer);
	printf("[%ld] panic:    mepc     = %8.8lx %58s", mhartid, csr_read_mepc(), trailer);
	printf("[%ld] panic:    mcause   = %8.8lx ", mhartid, csr_read_mcause());
	len = dump_tcause(csr_read_mcause());
	printf("%*s", 58-len, trailer);
	printf("[%ld] panic:    mtval    = %8.8lx %58s", mhartid, csr_read_mtval(), trailer);
	printf("[%ld] panic:    mtvec    = %8.8lx %58s", mhartid, csr_read_mtvec(), trailer);

	printf("[%ld] panic:    medeleg  = %8.8lx %58s", mhartid, csr_read_medeleg(), trailer);
	printf("[%ld] panic:    mideleg  = %8.8lx %58s", mhartid, csr_read_mideleg(), trailer);

	printf("[%ld] panic:    sstatus  = %8.8lx ", mhartid, csr_read_sstatus());
	len = dump_status(csr_read_sstatus());
	printf("%*s", 58-len, trailer);
	printf("[%ld] panic:    sie      = %8.8lx %58s", mhartid, csr_read_sie(), trailer);
	printf("[%ld] panic:    sip      = %8.8lx %58s", mhartid, csr_read_sip(), trailer);
	printf("[%ld] panic:    sepc     = %8.8lx %58s", mhartid, csr_read_sepc(), trailer);
	printf("[%ld] panic:    scause   = %8.8lx ", mhartid, csr_read_scause());
	len = dump_tcause(csr_read_scause());
	printf("%*s", 58-len, trailer);
	printf("[%ld] panic:    stval    = %8.8lx %58s", mhartid, csr_read_stval(), trailer);
	printf("[%ld] panic:    stvec    = %8.8lx %58s", mhartid, csr_read_stvec(), trailer);

	printf("[%ld] panic:    satp     = %8.8lx ", mhartid, csr_read_satp());
	len = dump_satp(csr_read_satp());
	printf("%*s", 58-len, trailer);
	//printf("[%ld] sedeleg = %8.8lx\n", mhartid, csr_read_sedeleg());
	//printf("[%ld] sideleg = %8.8lx\n", mhartid, csr_read_sideleg());



#if 0
	// waste of space to print this every time...
	printf("[%ld] panic:    mvendorid= %8.8lx\n", mhartid, csr_read_mvendorid());
	printf("[%ld] panic:    marchid  = %8.8lx\n", mhartid, csr_read_marchid());
	printf("[%ld] panic:    mimpid   = %8.8lx\n", mhartid, csr_read_mimpid());
	printf("[%ld] panic:    mhartid  = %8.8lx\n", mhartid, csr_read_mhartid());
#endif
	printf("[%ld] panic:    misa     = %8.8lx ", mhartid, csr_read_misa());
	len = dump_misa(csr_read_misa());
	printf("%*s", 58-len, trailer);

	printf("[%lu] panic: **** END ******************************************************************* [%lu]\n", mhartid, mhartid);


#if 0
	hexdump((void*)0x808161b0, 1024);
	hexdump((void*)0x84100800, 0x110);
#endif
}




/**
* Dump the state of the hart & halt it.
*
* XXX probably need to do this using ebreak or something that assures we
*	end up in machine mode so we can halt all interrupts.
*
* @bug XXX Redesign this into an alternate of the naked handler but 
*	with a message arg.
***************************************************************************/
__attribute__((noreturn)) void panic(const char *msg)
{
	panic_dump(msg);	// XXX this will print garbage register values

	// halt the hart here!!!
	csr_clear_mstatus(MSTATUS_MIE|MSTATUS_SIE);		// disable all IRQs
	while(1)
		asm volatile("wfi");
}



__attribute__ ((naked)) void panic_naked(void)
{
	// Save every register so they can be dumped out later

	asm volatile(".option push");
	asm volatile(".option norelax");

	asm volatile("csrrw x0,mscratch,x2");		// store x

	asm volatile("la	x2,saved_xregs");		//  ... then use it as a base register
	asm volatile("sw	x1,0x004(x2)");			// save x1
	asm volatile("csrrs	x1,mscratch,x0");		// copy original x2 into x1 (& leave in mscratch)
	asm volatile("sw	x1,0x008(x2)");			// save original x2
	asm volatile("sw	x3,0x00c(x2)");			// save x3
	asm volatile("sw	x4,0x010(x2)");

	// Disable IRQs as soon as reasonable
	asm volatile("csrrci x1,mstatus,0x0a");		// disable IRQs (are already be off if got here via IRQ/trap)
	asm volatile("la	x2,saved_mstatus");		// XXX figure out how/when to dump this vs the current one
	asm volatile("sw	x1,0(x2)");

#if 0
	asm volatile("la	x2,panic_stack+4096");	// XXX
	printf("panic check 2222!\n");				// XXX
#endif

	// Finish saving the machine's state
	asm volatile("la	x2,saved_xregs");

	asm volatile("sw	x5,0x014(x2)");
	asm volatile("sw	x6,0x018(x2)");
	asm volatile("sw	x7,0x01c(x2)");
	asm volatile("sw	x8,0x020(x2)");
	asm volatile("sw	x9,0x024(x2)");
	asm volatile("sw	x10,0x028(x2)");
	asm volatile("sw	x11,0x02c(x2)");
	asm volatile("sw	x12,0x030(x2)");
	asm volatile("sw	x13,0x034(x2)");
	asm volatile("sw	x14,0x038(x2)");
	asm volatile("sw	x15,0x03c(x2)");
	asm volatile("sw	x16,0x040(x2)");
	asm volatile("sw	x17,0x044(x2)");
	asm volatile("sw	x18,0x048(x2)");
	asm volatile("sw	x19,0x04c(x2)");
	asm volatile("sw	x20,0x050(x2)");
	asm volatile("sw	x21,0x054(x2)");
	asm volatile("sw	x22,0x058(x2)");
	asm volatile("sw	x23,0x05c(x2)");
	asm volatile("sw	x24,0x060(x2)");
	asm volatile("sw	x25,0x064(x2)");
	asm volatile("sw	x26,0x068(x2)");
	asm volatile("sw	x27,0x06c(x2)");
	asm volatile("sw	x28,0x070(x2)");
	asm volatile("sw	x29,0x074(x2)");
	asm volatile("sw	x30,0x078(x2)");
	asm volatile("sw	x31,0x07c(x2)");

	// Set up viable context since we don't really know how we got here
	asm volatile("la	gp,__global_pointer$");
	asm volatile("la	x2,panic_stack+(1024*4*16)");

	// The gcc docs say that we can ONLY count on asm instructions working in a naked func
	asm volatile("addi	x10,x0,0");			// set the msg arg to null
	asm volatile("jal	x1,panic_dump");

	asm volatile("li    x1,-1");
	asm volatile("csrrc x0,mie,x1");        // disable ALL irqs and traps

	asm volatile("wfi");					// wait for an IRQ (seize the hart)
	asm volatile("j  .-4");					// if the impossible happens...

	asm volatile(".option pop");
}
