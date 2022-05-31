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


#include "hexdump.h"
#include "lock.h"
#include "trap.h"
#include "csr.h"
#include "clint.h"
#include "plic.h"
#include "msi.h"
#include "ns16550.h"
#include "panic.h"
#include "sifive_test.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static uint32_t biglock;


/**
***************************************************************************/
int main()
{
	lock(&biglock);		// XXX hack to serialize the harts
	panic_dump("entering main()");


	trap_init();		// set up the mtvec (all harts need to do this)

// XXX barrier so no IRQs are generated before all the harts are ready to deal with them! XXX



	clint_init();		// only one hart need do this
	msi_init();			// only one hart need do this

	uint32_t mhartid = csr_read_mhartid();
	uint32_t misa = csr_read_misa();

	lock(&stdio_lock);	// LLLLLLLLLLLLLLLLL

	printf("mhartid=0x%8.8lx, misa=0x%8.8lx\n", mhartid, misa);

	printf("rv32");
	for (uint8_t i=0; i<26; ++i)
	{
		if (misa&1)
			printf("%c", 'a'+i);
		misa>>=1;
	}
	printf("\n");

	if (mhartid==0)
	{
		extern uint8_t __ram_origin__[];
		extern uint8_t __ram_end__[];
		printf("__ram_origin__=%p\n", __ram_origin__);
		printf("   __ram_end__=%p\n", __ram_end__);
#if 0
		printf("Top of the stack:\n");
		hexdump(__ram_end__-0x2000, 0x1000);
#endif
#if 0
		uint32_t *virt = (uint32_t*)0x10008000;
		printf("virtio_mmio@10008000:\n");
		for (int i=0; i<16; ++i)
			printf("%8.8lx ", virt[i]);
		printf("\n");
#endif
	}

	const uint32_t target = 2;
	if (mhartid==target)
	{
		ns16550_enable_irq(1);					// tell the UART to generate IRQs when RX data is ready

		plic_init();	// XXX only one hart can init the plic!
		plic_set_enable(PLIC_IRQ_UART, mhartid);	// route the IRQ from the UART into the target hart
#if 1
		plic_set_enable(PLIC_IRQ_UART, 1);	// route the UART to multiple harts at the same time
#endif
		plic_dump();

		// send a machine software interrupt to hart 0
		//CLINT0.msip[0] = 1;

	}

	if (mhartid==0)
	{
		clint_set_mtimecmp_period(0x989680, mhartid);	// 1HZ (see the timebase-frequency in the dtb)
		csr_set_mie(MIE_MTIE);								// enable the timer interrupts
	}
#if 1
	if (mhartid==3)
	{
		// run it at a different rate...
		clint_set_mtimecmp_period(0x989680*3.14, mhartid);
		csr_set_mie(MIE_MTIE);
	}
#endif

	csr_set_mie(MIE_MEIE);					// enable the external interrupts (PLIC/UART)
	csr_set_mie(MIE_MSIE);					// enable software interrupts

	csr_set_mstatus(MSTATUS_MIE);			// enable all IRQs

	printf("[%lu] done\n", mhartid);
	printf("[%lu] mtimecmp = %llu\n", mhartid, clint_get_mtimecmp(mhartid));

	unlock(&stdio_lock);	// LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL

	panic_dump("after setup");

	unlock(&biglock);		// XXX hack to serialize the harts

	//qemu_exit(0);

#if 1
	while(1)
	{
#if 0
		lock(&stdio_lock);
		printf("[%lu] wfi...\n", mhartid);
		unlock(&stdio_lock);
#endif
		asm volatile ("wfi");
#if 1
		if (mhartid==0)
		{
			clint_set_msip(7);	// send a software IRQ to hart 7
			clint_set_msip(6);	// send a software IRQ to hart 6
		}
#endif
#if 0
		plic_dump();
		ns16550_dump();
		if (ns16550_rx_ready())
		{
			uint8_t ch = ns16550_rx();
			printf("[%lu] Got char %02x: '%c'\n", mhartid, ch, ch);
		}
#endif

#if 0
		lock(&stdio_lock);
		printf("[%lu] wfi...\n", mhartid);
		unlock(&stdio_lock);

		asm volatile ("wfi");
#if 0
		lock(&stdio_lock);
		printf("looping...\n");
		unlock(&stdio_lock);
#else
		uint64_t mtime = clint_get_mtime();
		uint32_t hh = mtime/10000000/60/60;
		uint32_t mm = (mtime/10000000/60)%60;
		uint32_t ss = (mtime/10000000)%60;
		lock(&stdio_lock);
		printf("[%lu] TICK time=%llu, cmp=%llu, %lu:%02lu:%02lu\n", mhartid, mtime, CLINT0.mtimecmp[mhartid], hh, mm, ss);
		unlock(&stdio_lock);
#endif
		// (*(char*)0x4) = 99;	// XXX cause a write-fault to an illegal memory address
		//asm volatile("ebreak");
#endif

	}
#endif

	return 0;
}
