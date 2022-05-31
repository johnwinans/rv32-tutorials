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


#include "dbg.h"
#include "sv32.h"
#include "csr.h"
#include "hexdump.h"

#include <stdio.h>


/**
* Add a mapping for a single superpage tot he given page table.
***************************************************************************/
static void sv32_map(
	uint32_t *proot,	///< The address of the root page table
	uint32_t virt,		///< The virtual address to map from (must be multiple of 0x00400000)
	uint32_t phys,		///< The physical address to map to (must be multiple of 0x00400000)
	uint32_t mode)		///< The mode bits that are ORd into the mapping
{
	proot[virt>>22] = (phys>>2) | mode;
}


/**
* Set the given range of superpages to have a 1:1 mapping.
***************************************************************************/
static void sv32_make_transparent(
	uint32_t *proot,	///< The address of the root page table
	uint32_t phys,		///< The physical address to map (must be multiple of 0x00400000)
	uint32_t pages,		///< The number of superpages to map
	uint32_t mode)		///< The mode bits that are ORd into the mapping
{
	for (uint32_t p = phys>>22; pages>0; --pages, ++p)
	{
		sv32_map(proot, phys, phys, mode);
		phys += (1<<22);		// point to next 4MB page address
	}
}


/**
***************************************************************************/
static void sv32_init_root_ptable(
	uint32_t *proot)			///< the address of the table to create
{
	for (uint32_t p = 0; p<1024; ++p)
		proot[p] = 0;			// invalid superpage
}


/**
* Build a page table with the following mapping: 
*
*  02000000-0200ffff   riscv,clint
*  0c000000-0fffffff   riscv,plic
*  10000000-100000ff   ns16550a (UART)
*  10000100-10000fff   unused
*  10001000-10001fff   virtio,mmio
*  10002000-10002fff   virtio,mmio
*  10003000-10003fff   virtio,mmio
*  10004000-10004fff   virtio,mmio
*  10005000-10005fff   virtio,mmio
*  10006000-10006fff   virtio,mmio
*  10007000-10007fff   virtio,mmio
*  10008000-10008fff   virtio,mmio
*
* The goal is to map only those pages that the supervisor and user mode
* code is allowed to access.
***************************************************************************/
void sv32_setup()
{
	const uint32_t user_mode_rwx = SV32_PTE_V|SV32_PTE_R|SV32_PTE_W|SV32_PTE_X|SV32_PTE_U;
	const uint32_t super_mode_rwx = SV32_PTE_V|SV32_PTE_R|SV32_PTE_W|SV32_PTE_X;
	const uint32_t super_mode_rw = SV32_PTE_V|SV32_PTE_R|SV32_PTE_W;

	// construct a page table for an address space
	// mapping: 0x90000000 - 0x903fffff = 0x80800000 - 0x80afffff;		// 4MB 

	// root page table
	uint32_t *proot = (uint32_t*)0x84100000;
	sv32_init_root_ptable(proot);		// make all pages invalid/off

	// I/O region and first part of RAM for supervisor mode access only
	sv32_make_transparent(proot, 0x02000000, 1, super_mode_rw);			// clint
	sv32_make_transparent(proot, 0x0c000000, 0x40>>2, super_mode_rw);	// plic
	sv32_make_transparent(proot, 0x10000000, 1, super_mode_rw);			// UART & virtio

	sv32_make_transparent(proot, 0x80000000, 1, super_mode_rwx);		// RAM

	// The stack (supervosor mode only)
	sv32_make_transparent(proot, 0x80400000, 1, super_mode_rw);


	// the user mode virtual memory page
	sv32_map(proot, 0x90000000, 0x80800000, user_mode_rwx);


	hexdump(proot, 0x1000);	
	//hexdump(proot-4, 0x1020);	// over-dump to make sure we see it begin & end

#if 1
	// set the satp register for this hart (this turns 'on' the virtual memory)
	uint32_t root = 0x84100000;
	uint32_t asid = 1;					// note that ASID values have a per-hart scope
	uint32_t mode = 1;					// mode 1 = sv32

	uint32_t satp = (root>>12)|(asid<<22)|(mode<<31);
 	csr_write_satp(satp);
	asm volatile("sfence.vma");
#endif
}
