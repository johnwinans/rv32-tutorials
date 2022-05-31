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
#include "clint.h"
#include "ns16550.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

uint32_t b1;
uint32_t b2[4];

extern uint8_t __ram_origin[];
extern uint8_t __ram_end[];

/**
***************************************************************************/
int main()
{
	// set mtvec to handle all traps
	trap_init();

	uint32_t my_mhartid;	// purposly named different than the csr to reduce confusion 
	asm volatile("csrrs %0,mhartid,x0" : "=r"(my_mhartid) );	// read the mhartid register
	uint32_t my_misa;
	asm volatile("csrrs %0,misa,x0" : "=r"(my_misa) );	

	lock(&stdio_lock);
	printf("mhartid=0x%8.8x, misa=0x%8.8x\n", (unsigned int)my_mhartid, (unsigned int)my_misa);

	printf("rv32");
	for (uint8_t i=0; i<26; ++i)
	{
		if (my_misa&1)
			printf("%c", 'a'+i);
		my_misa>>=1;
	}
	printf("\n");


#if 1
	if (my_mhartid==0)
	{
		printf("__ram_origin=%p\n", __ram_origin);
		printf("__ram_end=%p\n", __ram_end);
#if 1
		printf("Top of the stack:\n");
		hexdump(__ram_end-0x2000, 0x1000);
#endif
	}

#endif

	printf("hart %lu done\n", my_mhartid);

#if 0
	for (int i=0; i<10; ++i)
	{
		lock(&stdio_lock);
		printf("mtime = %llu\n", CLINT_MTIME);
		unlock(&stdio_lock);
	}

	// try to generate an exception and see of the handler deals with it OK
	(*(char*)0x4) = 99;


	// warp into the default IRQ handler to dump the state of the hart
	//asm volatile("j trap_default");
#endif

	printf("mtimecmp = %llu\n", CLINT_MTIMECMP);

	//clint_set_mtimecmp_period(32768);		// 1HZ (the FE310-G002 lfclk runs at 32768 HZ)
	clint_set_mtimecmp_period(0x989680);	// 1HZ (see the timebase-frequency in the dtb)
	trap_set_mie(MIE_MTIE);					// enable the timer interrupts

	//ns16550_tx_flush();	// wait for the uart to finish printing anything from above!

	trap_set_mstatus(MSTATUS_MIE);			// enable IRQs

	unlock(&stdio_lock);

#if 1
	while(1)
	{
		asm volatile ("wfi");
#if 0
		printf("looping...\n");
#else
		uint64_t mtime = CLINT_MTIME;
		uint32_t hh = mtime/10000000/60/60;
		uint32_t mm = (mtime/10000000/60)%60;
		uint32_t ss = (mtime/10000000)%60;
		printf("TICK time=%llu, cmp=%llu, %lu:%02lu:%02lu\n", mtime, CLINT_MTIMECMP, hh, mm, ss);
#endif
		// (*(char*)0x4) = 99;	// XXX cause a write-fault to an illegal memory address
		//asm volatile("ebreak");
	}
#endif

	return 0;
}
