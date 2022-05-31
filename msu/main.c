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


#include "clint.h"
#include "trap.h"
#include "csr.h"
#include "panic.h"
#include "dbg.h"
#include "hexdump.h"

#include "sifive_test.h"

#include <stdio.h>

void user_mode()
{
	DBG("entered\n");
	(*(uint32_t*)0xe0e0e0e0) = 23;		// memory write fault?
	DBG("should not get here!\n");
}

void super_mode()
{
	DBG("entered\n");
	csr_set_sstatus(MSTATUS_MPP_U);
	csr_write_sepc((uint32_t)user_mode);
	DBG("executing sret...\n");
	asm volatile ("sret");

	(*(uint32_t*)0xf0f0f0f0) = 23;		// memory write fault?
}


void dump_devicetree()
{
	//uint32_t len = *(uint32_t*)0x1000;
	hexdump((void*)0x1000, 0x4000);
}


/**
***************************************************************************/
int main()
{
	trap_init();

	//dump_devicetree();

	//csr_set_mstatus(MSTATUS_MPP_U);
	csr_set_mstatus(MSTATUS_MPP_S);
	//csr_set_mstatus(MSTATUS_MPP_M);
	csr_write_mepc((uint32_t)super_mode);
	panic_dump("before mret...");		// this will make illegal accesses to mXXX CSRs!

	DBG("executing mret...\n");
	asm volatile ("mret");

#if 0
	csr_set_sstatus(MSTATUS_SPP);
	csr_write_sepc((uint32_t)super_mode);
	asm volatile ("sret");
#endif
	panic("we should not get here!");

#if 0
	clint_init();

	uint32_t mhartid = 0;								// because the clint handles multiple harts
	clint_set_mtimecmp_period(CLINT_MTIME_HZ, mhartid);	// 1HZ (see the timebase-frequency in the dtb)

	csr_write_mie(MIE_MTIE);							// enable /only/ the timer interrupts
	csr_set_mstatus(MSTATUS_MIE);						// enable all IRQs

	printf("[%lu] mtimecmp = %llu\n", mhartid, clint_get_mtimecmp(mhartid));

	while(1)
	{
		printf("[%lu] wfi...\n", mhartid);
		asm volatile ("wfi");	// this could return for all sorts of reasons other than the mtimer IRQ!!!!!

		printf("mstatus = 0x%8.8lx, mie = 0x%8.8lx, mip=0x%8.8lx\n", csr_read_mstatus(), csr_read_mie(), csr_read_mip());

		uint64_t mtime = clint_get_mtime();
		uint32_t hh = mtime/10000000/60/60;
		uint32_t mm = (mtime/10000000/60)%60;
		uint32_t ss = (mtime/10000000)%60;
		uint32_t nn = (mtime/10)%1000000;
		printf("time=0x%16.16llx, %lu:%02lu:%02lu.%6.6lu\n", mtime, hh, mm, ss, nn);

#if 0
		if (ss > 3)
			qemu_exit(0);
#endif
	}
#endif

	return 0;
}
