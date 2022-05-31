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

#include <stdio.h>

#define MTIMER_MAX_HARTS	(8)
static uint64_t mtimer_period[MTIMER_MAX_HARTS] = { 
	CLINT_MTIME_HZ,
	CLINT_MTIME_HZ,
	CLINT_MTIME_HZ,
	CLINT_MTIME_HZ,
	CLINT_MTIME_HZ,
	CLINT_MTIME_HZ,
	CLINT_MTIME_HZ,
	CLINT_MTIME_HZ
};

// mtimecmp is a 64-bit timeout value that will take over 1,000,000 
// years to overflow.

/**
* Enable an IRQ at dt into the future.  dt is measured in 1/10,000,000 seconds.
***************************************************************************/
void clint_set_mtimecmp_period(uint64_t dt, uint32_t hartid)
{
	if (hartid<MTIMER_MAX_HARTS)
	{
		mtimer_period[hartid] = dt;

    	//CLINT0.mtimecmp[hartid] = clint_get_mtime() + period;
    	//CLINT0.mtimecmp[hartid].u64 = dt;	// if it starts behind, it will get multiple IRQs until it catches up
		clint_set_mtimecmp(hartid, dt);		// if starting behind, it will get multiple IRQs until it catches up
	}
}


/**
* Called when a timer IRQ occurs.
*
* @note By advancing the mtimecmp value, rather setting it to mtime+period,
*	priodic interrupts will not fall behind.
*
* From rvisc-v priv insn manual: 
* A spurious timer interrupt might occur if an interrupt handler increments 
* mtimecmp then immediately returns, because MTIP might not yet have fallen 
* in the interim. All software should be written to assume this event is 
* possible, but most software should assume this event is extremely
* unlikely. It is almost always more performant to incur an occasional 
* spurious timer interrupt than to poll MTIP until it falls.
*
* Also from the priv insn manual on how to update mtimecmp on a 32-bit system
* ~~~~
*	# New comparand is in a1:a0.
*	li t0, -1
*	la t1, mtimecmp
*	sw t0, 0(t1) # No smaller than old value.
*	sw a1, 4(t1) # No smaller than new value.
*	sw a0, 0(t1) # New value.
* ~~~~
***************************************************************************/
static void clint_mtime_irq_handler(void)
{
	uint32_t mhartid = csr_read_mhartid();

	// A problem here is if another hart is diddling with our mtimecmp at the same time.
	// ... probably a bad design.. but I will err on the side of caution here.
	uint64_t mtc = clint_get_mtimecmp(mhartid);

	if (clint_get_mtime() > mtc)			// check for possible IRQ glitches 
	{
		mtc += mtimer_period[mhartid];
    	CLINT0.mtimecmp[mhartid].u64 = mtc;
	}

	// The priv insn manual notes that mtimecmp+=n can glitch.  But we know 
	// that MTIP is high in order to get here in the first place.  So the 
	// only glitch that this can cause would be for it to go off and then 
	// back on again... which would not be a problem.
	//
	// The priv insn manual narrative is likely referring to adjusting mtimecmp
	// from OUTSIDE of an IRQ handler.

#if 1
	// XXX It is just WRONG to do such slow-moving things in an IRQ handler!!!
	// XXX it is likely that the stdio buffering is not MT-safe!
	uint64_t mtime = clint_get_mtime();
	uint32_t hh = mtime/10000000/60/60;
	uint32_t mm = (mtime/10000000/60)%60;
	uint32_t ss = (mtime/10000000)%60;
    printf("[%lu] clint_mtime_irq_handler() irq_7(): time=0x%16.16llx (%llu), cmp=%llu, %lu:%02lu:%02lu\n", mhartid, mtime, mtime, mtc, hh, mm, ss);
#endif
}

/**
* Register the interrupt handler for mtimer interrupts.
***************************************************************************/
void clint_init()
{
	trap_register_interrupt_handler(MCAUSE_IRQ_MACHINE_TIMER, clint_mtime_irq_handler);
}
