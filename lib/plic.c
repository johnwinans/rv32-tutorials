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


#include "plic.h"
#include "trap.h"
#include "panic.h"

#include <stdio.h>

static void plic_machine_irq_handler();
//static void plic_software_irq_handler();

/**
* Reset/disable all IRQs and levels
***************************************************************************/
void plic_init()
{
	// Set all thresholds to the highest level to all levels
	for (int i=0; i<PLIC_NUM_QEMU_CONTEXT; ++i)
		PLIC->prio_claim[i].prio_threshold = 7;

	// set all priorities to zero for each source (disabling them)
	for (int i=0; i<PLIC_NUM_QEMU_SOURCE; ++i)
		PLIC->priority[i] = 0;

	// set all source enable bits to zero (disabling them all)
	for (int i=0; i<PLIC_NUM_QEMU_CONTEXT; ++i)
        for (int j=0; j<(PLIC_NUM_QEMU_SOURCE+31)/32; ++j)
			PLIC->enable[i][j] = 0;

	// Register a handler for all machine external IRQs 
	trap_register_interrupt_handler(MCAUSE_IRQ_MACHINE_EXT, plic_machine_irq_handler);

	// Register a handler for all supervisor external IRQs 
	//trap_register_interrupt_handler(MCAUSE_IRQ_SUPERVISOR_EXT, plic_supervisor_irq_handler);
}

/**
* Enable/disable IRQs.
***************************************************************************/
void plic_set_enable(uint32_t enable, uint32_t hartid)
{
	// the sifive plic is a type "MS" where each target is a machine/supervisor pair of contexts
	uint32_t context = hartid*2;	

    // assign a non-zero priority
	PLIC->priority[enable] = 1;							// 0 = disabled
	PLIC->enable[context][enable/32] = 1<<(enable%32);	// enable the IRQ source
	PLIC->prio_claim[context].prio_threshold = 0;		// enable ALL priorities for this context
}

/**
* Dump the state of the PLIC
***************************************************************************/
void plic_dump()
{
	uint32_t mip;
	uint32_t mie;
	asm volatile("csrrs %0,mip,x0" : "=r"(mip) );
	asm volatile("csrrs %0,mie,x0" : "=r"(mie) );
	printf("mip=0x%8.8lx, mie=0x%8.8lx\n", mip, mie);

	printf("PLIC_PRIORITY:\n");
	for (int i=0; i<PLIC_NUM_QEMU_SOURCE; ++i)
		printf("    %2d, %p: 0x%8.8lx%c\n", i, &PLIC->priority[i], PLIC->priority[i], (PLIC->priority[i]>0)?'<':' ');

	printf("PLIC_PENDING:\n");
	for (int i=0; i<(PLIC_NUM_QEMU_SOURCE+31)/32; ++i)
		printf("    %2d, %p: 0x%8.8lx%c\n", i*32, &PLIC->pending[i], PLIC->pending[i], (PLIC->pending[i])?'<':' ');

	printf("PLIC_ENABLE (per-context):\n");
    for (int i=0; i<PLIC_NUM_QEMU_CONTEXT; ++i)
    {
#if 1
        printf("    %2d, %p:", i, &PLIC->enable[i][0]);
        for (int j=0; j<(PLIC_NUM_QEMU_SOURCE+31)/32; ++j)
            printf(" 0x%8.8lx%c ", PLIC->enable[i][j], (PLIC->enable[i][j]==0)?' ':'<');
		printf("\n");
#else
        printf("    context=%d:\n", i);
        for (int j=0; j<(PLIC_NUM_QEMU_SOURCE+31)/32; ++j)
            printf("        %2d, %p: 0x%8.8lx\n", j*32, &PLIC->enable[i][j], PLIC->enable[i][j]);
#endif
    }

	printf("PLIC_THRESHOLD (per-context):\n");
	for (int i=0; i<PLIC_NUM_QEMU_CONTEXT; ++i)
		printf("    %2d, %p: 0x%8.8lx%c\n", i, &PLIC->prio_claim[i].prio_threshold, PLIC->prio_claim[i].prio_threshold, (PLIC->prio_claim[i].prio_threshold==7)?' ':'<');
}


void plic_null(void)
{
	panic("unregistered PLIC handler");
}

#define WEAK_ALIAS(x) __attribute__ ((weak, alias(#x)))
void plic_0(void) WEAK_ALIAS(plic_null);
void plic_1(void) WEAK_ALIAS(plic_null);
void plic_2(void) WEAK_ALIAS(plic_null);
void plic_3(void) WEAK_ALIAS(plic_null);
void plic_4(void) WEAK_ALIAS(plic_null);
void plic_5(void) WEAK_ALIAS(plic_null);
void plic_6(void) WEAK_ALIAS(plic_null);
void plic_7(void) WEAK_ALIAS(plic_null);
void plic_8(void) WEAK_ALIAS(plic_null);
void plic_9(void) WEAK_ALIAS(plic_null);
void plic_10(void) WEAK_ALIAS(plic_null);

static void(*plic_vectors[])(void) = {
	plic_0,
	plic_1,
	plic_2,
	plic_3,
	plic_4,
	plic_5,
	plic_6,
	plic_7,
	plic_8,
	plic_9,
	plic_10
};



/**
* Called when an external machine IRQ occurs.
***************************************************************************/
static void plic_machine_irq_handler()
{
	uint32_t mhartid;
	asm volatile("csrrs %0,mhartid,x0" : "=r"(mhartid) );

	// figure out where it came from & dispatch to the device handler
	uint32_t context = mhartid*2;	// double it for an MS type plic
	uint32_t claim = PLIC->prio_claim[context].claim_complete;

#if 0
	// XXX It is just WRONG to do slow-moving things in an IRQ handler!!!
    printf("[%lu] irq_11(): context=%lu, claim=%lu\n", mhartid, context, claim);
#endif

	// If multiple harts could receive the IRQ, only 1 will get a non-zero claim
	if (claim!=0)
	{
#if 1
    	printf("[%lu] irq_11(): context=%lu, claim=%lu\n", mhartid, context, claim);
#endif
		// handle the IRQ here... 
		if (claim>12)
			plic_null();
		else
			plic_vectors[claim]();

		PLIC->prio_claim[context].claim_complete = claim;
	}
}
