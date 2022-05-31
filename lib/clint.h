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


#ifndef clint_H
#define clint_H

#include <stdint.h>


// The clint looks like this in the device tree:
//
//	clint@2000000 {
//		interrupts-extended = <0x10 0x03 0x10 0x07 0x0e 0x03 0x0e 0x07 0x0c 0x03 0x0c 0x07 0x0a 0x03 0x0a 0x07 0x08 0x03 0x08 0x07 0x06 0x03 0x06 0x07 0x04 0x03 0x04 0x07 0x02 0x03 0x02 0x07>;
//		reg = <0x00 0x2000000 0x00 0x10000>;
//		compatible = "riscv,clint0";
//	};
//
// ... and probably means that it is connected to IRQs 0x03 and 0x07 on each of the harts:
//
// IRQ 0x03 = Machine Software IRQ
// IRQ 0x07 = Machine Timer IRQ
//
// In order to generate a software interrupt on a specific hart, write the
// value 1 into the msip register for the given hart.  see clint_set_msip()
// For example to interrupt hart 5:
//
// CLINT0.msip[5] = 1;	
//
// Note that the interrupt will be asserted until the msip register value
// is set back to 0.
// 





// From priv insn manual on how to update mtimecmp on a 32-bit system
//
// # New comparand is in a1:a0.
// li t0, -1
// la t1, mtimecmp
// sw t0, 0(t1) # No smaller than old value.
// sw a1, 4(t1) # No smaller than new value.
// sw a0, 0(t1) # New value.
//
//



/**
* CLINT_MTIME_HZ is the timebase-frequency (from the qemu dtb)
***************************************************************************/
#define CLINT_MTIME_HZ	((uint64_t)0x989680)

/**
* The CLINT device has one msip and one mtimecmp for each hart.
*
* Writing a 1 into the msip will cause the clint to assert the software 
* interrupt on the given hart.  (See the RISC-V doc on the mip and mie 
* registers.)
*
* Each hart has its own mtimecmp.  When mtime > mtimecmp, the clint will
* assert the timer interrupt on the given hart.  (See the RISC-V doc on 
* the mtime and mtimecmp registers.)
***************************************************************************/
struct clint_t
{
	volatile uint32_t	msip[0x1000];
	union
	{
		volatile uint64_t	u64;
		struct 
		{
			volatile uint32_t	lo;		// little-endian
			volatile uint32_t	hi;
		} u32;
	} mtimecmp[0x0fff];
	union
	{
		volatile uint64_t	u64;
		struct 
		{
			volatile uint32_t	lo;		// little-endian
			volatile uint32_t	hi;
		} u32;
	} mtime;
};

#define CLINT0 (*(struct clint_t*)(0x2000000))       

void clint_set_mtimecmp_period(uint64_t dt, uint32_t hartid);


/**
* Read the 64-bit mtime register... carefully so that an intermediate
* carry from 31->32 does not mess up reading a 64-bit value as two 32-bit 
* values. (This is the same advice given by the riscv priv spec for
* reading 64-bit csrs.)
***************************************************************************/
inline __attribute__((always_inline)) uint64_t clint_get_mtime()
{
// gcc -O will generate the same code for either of the two following implementations
#if 0
    union
    {
        uint64_t    v64;
        struct
        {
            uint32_t    lo;
            uint32_t    hi;
        } v32;
    } v;
    uint32_t    hi2;

	do
	{
		v.v32.hi = CLINT0.mtime.u32.hi;
		v.v32.lo = CLINT0.mtime.u32.lo;
		hi2 = CLINT0.mtime.u32.hi;
	} while (hi2 != v.v32.hi);
	return v.v64;
#else
	uint32_t hi1;
	uint32_t lo;
	uint32_t hi2;
	do
	{
		hi1 = CLINT0.mtime.u32.hi;
		lo = CLINT0.mtime.u32.lo;
		hi2 = CLINT0.mtime.u32.hi;
	}
	while (hi1 != hi2);

	return ((uint64_t)hi1)<<32 | lo;
#endif
}

/**
* The motivation for this wrapper is consistency with clint_get_mtime
***************************************************************************/
inline __attribute__((always_inline)) uint64_t clint_get_mtimecmp(uint32_t hartid)
{
	// Note that since mtimecmp is memory mapped, it could be changed
	// by an IRQ on this same hart while we are reading it here.
	// If the only changes are to move it forward, this should work OK
	// else this should disable the IRQs and do a simple read.
	// (harts diddling with different mtimecmp values is not likely a 
	// problem as they should not be doing that.) 

	uint32_t hi1;
	uint32_t lo;
	uint32_t hi2;
	do
	{
		hi1 = CLINT0.mtimecmp[hartid].u32.hi;
		lo = CLINT0.mtimecmp[hartid].u32.lo;
		hi2 = CLINT0.mtimecmp[hartid].u32.hi;
	}
	while (hi1 != hi2);

	return ((uint64_t)hi1)<<32 | lo;
}
/**
* carefully set the mtimecmp value to prevent possible glitching.
***************************************************************************/
inline __attribute__((always_inline)) void clint_set_mtimecmp(uint32_t hartid, uint64_t t)
{
	CLINT0.mtimecmp[hartid].u32.lo = 0xffffffff;
	CLINT0.mtimecmp[hartid].u32.hi = t>>32;
	CLINT0.mtimecmp[hartid].u32.lo = t & 0x0ffffffff;
}

/**
* Send a software interrupt to the given hart.
***************************************************************************/
inline __attribute__((always_inline)) void clint_set_msip(uint32_t hart)
{
    CLINT0.msip[hart] = 1;
}

/**
* Initialize the clint.
* This will register an interrupt handler for IRQ 7.
***************************************************************************/
void clint_init();


#endif
