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


#include "trap.h"
#include "lock.h"
#include "clint.h"

#include <stdio.h>

#define WEAK_ALIAS(x) __attribute__ ((weak, alias(#x)))

void trap_default(void);
void irq_0(void) WEAK_ALIAS(trap_default);
void irq_1(void) WEAK_ALIAS(trap_default);
void irq_2(void) WEAK_ALIAS(trap_default);
void irq_3(void) WEAK_ALIAS(trap_default);
void irq_4(void) WEAK_ALIAS(trap_default);
void irq_5(void) WEAK_ALIAS(trap_default);
void irq_6(void) WEAK_ALIAS(trap_default);
void irq_7(void) WEAK_ALIAS(trap_default);
void irq_8(void) WEAK_ALIAS(trap_default);
void irq_9(void) WEAK_ALIAS(trap_default);
void irq_10(void) WEAK_ALIAS(trap_default);
void irq_11(void) WEAK_ALIAS(trap_default);
void irq_12(void) WEAK_ALIAS(trap_default);
void irq_13(void) WEAK_ALIAS(trap_default);
void irq_14(void) WEAK_ALIAS(trap_default);
void irq_15(void) WEAK_ALIAS(trap_default);

/**
***************************************************************************/
void trap_init()
{
	extern char irq_vector_table[];
	uint32_t val = ((uint32_t)irq_vector_table)|1;		// set mode to vectored
	asm volatile("csrrs x0,mtvec,%0" : : "r"(val));
}





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


uint32_t saved_xregs[32];
//static double saved_fregs[32];


void trap_default_dump()
{
	uint32_t mhartid;
	asm volatile("csrrs %0,mhartid,x0" : "=r"(mhartid) );	// read the mhartid register

	lock(&stdio_lock);
	printf("[%lu] ", mhartid);

	uint32_t reg;
	asm volatile("csrrs %0,mcause,x0" : "=r"(reg) );
	if (reg&0x80000000)
	{
		reg &= 0x7fffffff;
		printf("IRQ %lu: ", reg);
		if (reg > 15)
			printf("Designated for platform use\n");
		else
		{
			printf("%s\n", mcause_irq[reg]);
			//irq_7();
		}
	}
	else
	{
		printf("TRAP %lu: ", reg);
		if (reg > 15)
			printf("Reserved/Designated for custom use\n");
		else
			printf("%s\n", mcause_trap[reg]);
	}


    for (int i=0; i<32; ++i)
    {
        if (i%8 == 0)
			printf("%s[%lu] %sx%d", (i!=0?"\n":""), mhartid, (i<10?" ":""), i);
        else if (i%4 == 0)
			printf(" ");
		printf(" %8.8x", (unsigned int)saved_xregs[i]);
    }
	printf("\n");

	asm volatile("csrrs %0,mepc,x0" : "=r"(reg) );
	printf("    mepc = %8.8x\n", (unsigned int)reg);
	asm volatile("csrrs %0,mcause,x0" : "=r"(reg) );
	printf("    mcause = %8.8x\n", (unsigned int)reg);
	asm volatile("csrrs %0,mtval,x0" : "=r"(reg) );
	printf("    mtval = %8.8x\n", (unsigned int)reg);
	asm volatile("csrrs %0,mip,x0" : "=r"(reg) );
	printf("    mip = %8.8x\n", (unsigned int)reg);

#if 0
	asm volatile("csrrs %0,mtinst,x0" : "=r"(reg) );
	printf("    mtinst = %8.8x\n", (unsigned int)reg);
	asm volatile("csrrs %0,mtval2,x0" : "=r"(reg) );
	printf("    mtval2 = %8.8x\n", (unsigned int)reg);
#endif

	asm volatile("csrrs %0,mstatus,x0" : "=r"(reg) );
	printf("    mstatus = %8.8x\n", (unsigned int)reg);
	asm volatile("csrrs %0,medeleg,x0" : "=r"(reg) );
	printf("    medeleg = %8.8x\n", (unsigned int)reg);
	asm volatile("csrrs %0,mideleg,x0" : "=r"(reg) );
	printf("    mideleg = %8.8x\n", (unsigned int)reg);
	asm volatile("csrrs %0,mie,x0" : "=r"(reg) );
	printf("    mie = %8.8x\n", (unsigned int)reg);
	asm volatile("csrrs %0,mtvec,x0" : "=r"(reg) );
	printf("    mtvec = %8.8x\n", (unsigned int)reg);

#if 0
	asm volatile("csrrs %0,timeh,x0" : "=r"(reg) );
	printf("    timeh = %8.8x\n", (unsigned int)reg);
	asm volatile("csrrs %0,time,x0" : "=r"(reg) );
	printf("    time = %8.8x\n", (unsigned int)reg);
#endif

	asm volatile("csrrs %0,mscratch,x0" : "=r"(reg) );
	printf("    mscratch = %8.8x\n", (unsigned int)reg);

	printf("    mtime = %llu\n", CLINT_MTIME);

	printf("halting...\n");

/* from gdb's info all-registers
pc             0x800000f8	0x800000f8 <main+84>
ustatus        Could not fetch register "ustatus"; remote failure reply 'E14'
fflags         0x0	RD:0 NV:0 DZ:0 OF:0 UF:0 NX:0
frm            0x0	FRM:0 [RNE (round to nearest; ties to even)]
fcsr           0x0	RD:0 NV:0 DZ:0 OF:0 UF:0 NX:0 FRM:0 [RNE (round to nearest; ties to even)]
uie            Could not fetch register "uie"; remote failure reply 'E14'
utvec          Could not fetch register "utvec"; remote failure reply 'E14'
uscratch       Could not fetch register "uscratch"; remote failure reply 'E14'
uepc           Could not fetch register "uepc"; remote failure reply 'E14'
ucause         Could not fetch register "ucause"; remote failure reply 'E14'
utval          Could not fetch register "utval"; remote failure reply 'E14'
uip            Could not fetch register "uip"; remote failure reply 'E14'
sstatus        0x0	0
sedeleg        Could not fetch register "sedeleg"; remote failure reply 'E14'
sideleg        Could not fetch register "sideleg"; remote failure reply 'E14'
sie            0x0	0
stvec          0x0	0
scounteren     0x0	0
sscratch       0x0	0
sepc           0x0	0
scause         0x0	0
stval          0x0	0
sip            0x0	0
satp           0x0	0
hstatus        Could not fetch register "hstatus"; remote failure reply 'E14'
hedeleg        Could not fetch register "hedeleg"; remote failure reply 'E14'
hideleg        Could not fetch register "hideleg"; remote failure reply 'E14'
hie            Could not fetch register "hie"; remote failure reply 'E14'
htvec          Could not fetch register "htvec"; remote failure reply 'E14'
hscratch       Could not fetch register "hscratch"; remote failure reply 'E14'
hepc           Could not fetch register "hepc"; remote failure reply 'E14'
hcause         Could not fetch register "hcause"; remote failure reply 'E14'
hbadaddr       Could not fetch register "hbadaddr"; remote failure reply 'E14'
hip            Could not fetch register "hip"; remote failure reply 'E14'
mstatus        0x0	SD:0 VM:00 MXR:0 PUM:0 MPRV:0 XS:0 FS:0 MPP:0 HPP:0 SPP:0 MPIE:0 HPIE:0 SPIE:0 UPIE:0 MIE:0 HIE:0 SIE:0 UIE:0
misa           0x4014112d	RV32ACDFIMSU
medeleg        0x0	0
mideleg        0x0	0
mie            0x0	0
mtvec          0x0	0
mcounteren     0x0	0
mucounteren    Could not fetch register "mucounteren"; remote failure reply 'E14'
mscounteren    0x0	0
mhcounteren    Could not fetch register "mhcounteren"; remote failure reply 'E14'

mhpmevent3-31     0x0	0

mscratch       0x0	0
mepc           0x0	0
mcause         0x0	0
mtval          0x0	0
mip            0x0	0

mbase          Could not fetch register "mbase"; remote failure reply 'E14'
mbound         Could not fetch register "mbound"; remote failure reply 'E14'
mibase         Could not fetch register "mibase"; remote failure reply 'E14'
mibound        Could not fetch register "mibound"; remote failure reply 'E14'
mdbase         Could not fetch register "mdbase"; remote failure reply 'E14'
mdbound        Could not fetch register "mdbound"; remote failure reply 'E14'

pmpcfg0        0x0	0
pmpcfg1        0x0	0
pmpcfg2        0x0	0
pmpcfg3        0x0	0
pmpaddr0-15       0x0	0

tselect        Could not fetch register "tselect"; remote failure reply 'E14'
tdata1         Could not fetch register "tdata1"; remote failure reply 'E14'
tdata2         Could not fetch register "tdata2"; remote failure reply 'E14'
tdata3         Could not fetch register "tdata3"; remote failure reply 'E14'
dcsr           Could not fetch register "dcsr"; remote failure reply 'E14'
dpc            Could not fetch register "dpc"; remote failure reply 'E14'
dscratch       Could not fetch register "dscratch"; remote failure reply 'E14'

mcycle         0x5de494d8	1575261400
minstret       0x5de7ea70	1575479920
mhpmcounter3-31   0x0	0

mcycleh        0x5819f	360863
minstreth      0x5819f	360863

mhpmcounter3h-31h	0x0	0

cycle          0x5e902760	1586505568
time           Could not fetch register "time"; remote failure reply 'E14'
instret        0x5e97143c	1586959420

hpmcounter3-31	0x0	0

cycleh         0x5819f	360863
timeh          Could not fetch register "timeh"; remote failure reply 'E14'
instreth       0x5819f	360863

hpmcounter3h-31h	0x0	0

mvendorid      0x0	0
marchid        0x0	0
mimpid         0x0	0
mhartid        0x0	0
priv           0x3	prv:3 [Machine]
*/



	unlock(&stdio_lock);
}


//void irq_default (void) __attribute__ ((interrupt ("machine")));
void trap_default (void) __attribute__ ((naked));
void trap_default (void)
{
	// save every register so they can be dumped out later
	asm volatile("csrrs x0,mscratch,x2");		// store x2 
	asm volatile("la	x2,saved_xregs");		//  ... then usit as a base register
	asm volatile("sw	x1,0x004(x2)");			// save x1
	asm volatile("csrrs	x1,mscratch,x0");		// restore original x2 into x1
	asm volatile("sw	x1,0x008(x2)");			// save original x2

	asm volatile("sw	x3,0x00c(x2)");			// save x3
	asm volatile("sw	x4,0x010(x2)");			// ...
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


	// set up an IRQ stack
	asm volatile("la	x2,__ram_end");
#if 1
	// The gcc docs say that we can ONLY count on asm instructions working in a naked func
	asm volatile("jal	x1,trap_default_dump");

	asm volatile("li	x1,-1");
	asm volatile("csrrc x0,mie,x1");		// disable ALL irqs and traps
	asm volatile("wfi");					// wait for an IRQ (seize the hart)
	asm volatile("j  .-4");					// if the impossible happens...
#else
	trap_default_dump();
	
	// Disable all IRQs and wait (this will seize the hart)
	trap_clear_mie(0xffffffff);
	while(1)
		asm volatile ("wfi");
#endif

}
