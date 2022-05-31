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
#include "trap.h"
#include "csr.h"
#include "dbg.h"

// This code uses the following definitions: (same as rv spec)
//
// trap - general term meaning either an exception or interrupt 
// exception - a trap that is synchronous, caused by an error in an instruction 
// interrupt - a trap that is asynchronous, caused by an external event

/**
* A default interupt handler for any unregistered trap cause codes.
***************************************************************************/
void trap_default_interrupt_handler(void)
{
	//panic("Unregistered soft_interrupt_table handler");
	panic_naked();
}

/**
* A default exception handler for any unregistered trap cause codes.
***************************************************************************/
void trap_default_exception_handler(void)
{
	//panic("Unregistered soft_exception_table handler");
	panic_naked();
}



// XXX if we have more than one hart, then we MIGHT need to give each its own vector table!

static void(*soft_interrupt_table[])(void) = {
    trap_default_interrupt_handler,		//  0 (not used for interrupts)
	trap_default_interrupt_handler,		//  1 Supervisor software interrupt
	trap_default_interrupt_handler,		//  2 Reserved
	trap_default_interrupt_handler,		//  3 Machine software interrupt
	trap_default_interrupt_handler,		//  4 Reserved
	trap_default_interrupt_handler,		//  5 Supervisor timer interrupt
	trap_default_interrupt_handler,		//  6 Reserved
	trap_default_interrupt_handler,		//  7 Machine timer interrupt
	trap_default_interrupt_handler,		//  8 Reserved
	trap_default_interrupt_handler,		//  9 Supervisor external interrupt
	trap_default_interrupt_handler,		// 10 Reserved
	trap_default_interrupt_handler,		// 11 Machine external interrupt
	trap_default_interrupt_handler,		// 12 Reserved
	trap_default_interrupt_handler,		// 13 Reserved
	trap_default_interrupt_handler,		// 14 Reserved
	trap_default_interrupt_handler		// 15 Reserved
};

static void(*soft_exception_table[])(void) = {
	trap_default_exception_handler,		//  0 Instruction address misaligned",
	trap_default_exception_handler,		//  1 Instruction access fault",
	trap_default_exception_handler,		//  2 Illegal instruction",
	trap_default_exception_handler,		//  3 Breakpoint",
	trap_default_exception_handler,		//  4 Load address misaligned",
	trap_default_exception_handler,		//  5 Load access fault",
	trap_default_exception_handler,		//  6 Store/AMO address misaligned",
	trap_default_exception_handler,		//  7 Store/AMO access fault",
	trap_default_exception_handler,		//  8 Environment call from U-mode",
	trap_default_exception_handler,		//  9 Environment call from S-mode",
	trap_default_exception_handler,		// 10 Reserved",
	trap_default_exception_handler,		// 11 Environment call from M-mode",
	trap_default_exception_handler,		// 12 Instruction page fault",
	trap_default_exception_handler,		// 13 Load page fault",
	trap_default_exception_handler,		// 14 Reserved",
	trap_default_exception_handler		// 15 Store/AMO page fault"
};

/**
* Register an interrupt handler for the given interrupt mcause 
* code/vector number.
* 
* @param i The interrupt mcause/exception code number.
* @param func The interrupt handler function to register.
*
* @note The handler should be a regular C function (that is, not a
*	function that has an interrupt attribute.)
***************************************************************************/
void trap_register_interrupt_handler(uint32_t i, void(*func)(void))
{
	if (i<sizeof(soft_interrupt_table)/sizeof(soft_interrupt_table[0]))
		soft_interrupt_table[i] = func;
}

/**
* Register an exception handler for the given exception mcause code/vector 
* number.
* 
* @param i The IRQ mcause/exception code number.
* @param func the IRQ handler function to register.
*
* @note The handler should be a regular C function (that is, not a
*	function that has an interrupt attribute.)
***************************************************************************/
void trap_register_exception_handler(uint32_t i, void(*func)(void))
{
	if (i<sizeof(soft_exception_table)/sizeof(soft_exception_table[0]))
		soft_exception_table[i] = func;
}


/**
* The one and only handler for all traps.
***************************************************************************/
// machine type means to use MRET to return (as opposed to SRET)  ??????
__attribute__ ((interrupt ("machine"))) 
void trap_direct_handler(void)
{
	int32_t mcause = csr_read_mcause();

	//asm volatile("csrr %0,mcause" : "=r"(mcause));

#if 0
	int32_t mepc;
	int32_t mtval;
	int32_t mtvec;
	asm volatile("csrr %0,mepc" : "=r"(mepc));
	asm volatile("csrr %0,mtval" : "=r"(mtval));
	asm volatile("csrr %0,mtvec" : "=r"(mtvec));
#endif

	if (mcause & 0x7ffffff0)
		panic("invalid IRQ mcause (exception code > 15)");

	uint32_t id = mcause & 0x0000000f;	// only 16 levels are allowed

	if (mcause < 0)			// Is an IRQ if mcause 'looks' negative
		soft_interrupt_table[id]();	
	else
		soft_exception_table[id]();
}


/**
* Call this to initialize the mtvec (needs to be done by each hart!)
***************************************************************************/
void trap_init()
{
#if 0
	DBG("WARNING: Hacked panic version!!\n");
	csr_write_mtvec(((uint32_t)panic_naked));
#else
	csr_write_mtvec(((uint32_t)trap_direct_handler));
#endif
}
