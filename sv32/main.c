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
#include "virtio.h"
#include "ecall.h"
#include "csr.h"
#include "dbg.h"
#include "panic.h"
#include "sv32.h"

#include "hexdump.h"
#include "sifive_test.h"

#include <stdio.h>




/**
* This code s entered in supervisor mode with virtual memory turned on.
***************************************************************************/
void super_mode()
{
    DBG("entered\n");		// this is OK because super-mode has 1:1 mapping from 0-0x80000000
	hexdump((void*)0x90000000, 0x100);	// the user program's virtual address!

#if 0
	uint32_t rc = ecall(1,2,3,4,5);	// test ecall 
	qemu_exit(rc);	// should not get here
#endif


#if 1
	// prepare to make the jump to user mode
    csr_set_sstatus(MSTATUS_MPP_U);
    //csr_write_sepc((uint32_t)user_mode);
    csr_write_sepc(0x90000000);	// the virtual address of the user prog's _start
    DBG("executing sret...\n");
    asm volatile ("sret");
#endif





	qemu_exit(0);	// should not get here
}



/**
***************************************************************************/
int main()
{
    trap_init();

    //dump_devicetree();
	virtio_dump((void*)0x10001000);
	virtio_dump((void*)0x10002000);
	virtio_dump((void*)0x10003000);

	DBG("hello from machine level\n");

	sv32_setup();

	hexdump((void*)0x80800000, 0x100);	// the user program load base address



    //csr_set_mstatus(MSTATUS_MPP_U);
    csr_set_mstatus(MSTATUS_MPP_S|MSTATUS_SUM);
    //csr_set_mstatus(MSTATUS_MPP_M);

    csr_write_mepc((uint32_t)super_mode);

    panic_dump("before mret...");       
    DBG("executing mret...\n");
    asm volatile ("mret");


	qemu_exit(1);	// should not get here


	uint32_t rc = ecall(1,2,3,4,5);

	return rc;
}
