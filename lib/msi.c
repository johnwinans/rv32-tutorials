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


#include "msi.h"
#include "trap.h"
#include "clint.h"

#include <stdio.h>


/**
* Machine software interrupt handler.
***************************************************************************/
static void msi_handler()
{
	uint32_t mhartid;
	asm volatile("csrrs %0,mhartid,x0" : "=r"(mhartid) );

	// XXX How do we ACK this thing?
	CLINT0.msip[mhartid] = 0;		// XXX is this correct???


	// ... figure out what/why we were interrupted


	printf("[%lu] irq_3(): Machine Software Interrupt\n", mhartid);
}


/**
* Call this to register the machine software interrupt handler.
***************************************************************************/
void msi_init()
{
	trap_register_interrupt_handler(MCAUSE_IRQ_MACHINE_SOFTWARE, msi_handler);
}

