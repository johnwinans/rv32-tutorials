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

#include <stdio.h>

static uint64_t period = 32767;

// mtimecmp is a 64-bit timeout value
// mtime is a 64-bit counter that runs at 32768 HZ
// Note the timer will overflow in about 17,851,025 years

/**
* Enable an IRQ at dt into the future.  dt is measured in 1/32768 seconds.
***************************************************************************/
void clint_set_mtimecmp_period(uint64_t dt)
{
	period = dt;
    CLINT_MTIMECMP = CLINT_MTIME + period;
#if 0
	printf("Setting period to %llu\n", period);
#endif
}


/**
* Called when a timer IRQ occurs.
*
* @note By advancing the mtimecmp value, rather setting it to mtime+period,
*	priodic interrupts will not fall behind.
***************************************************************************/
void irq_7(void) __attribute__ ((interrupt ("machine")));
void irq_7(void)
{
    CLINT_MTIMECMP += period;	// It is OK to let this glitch here (see priv insn manual) 
								//    ...because IRQs are currently disabled.

#if 1
	// XXX It is just WRONG to do the following in an IRQ handler!!!
	uint64_t mtime = CLINT_MTIME;
	uint32_t hh = mtime/10000000/60/60;
	uint32_t mm = (mtime/10000000/60)%60;
	uint32_t ss = (mtime/10000000)%60;
    printf("irq_7(): time=%llu, cmp=%llu, %lu:%02lu:%02lu\n", mtime, CLINT_MTIMECMP, hh, mm, ss);
#endif
}
