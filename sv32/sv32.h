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


#ifndef sv32_H
#define sv32_H

#include <stdint.h>


// Sv32 vitrtual memory
//
// virtual address format (32-bits wide):
//
// VPN[1] VPN[0] offset     <-- field name
//  10     10      12       <-- number of bits
//
// VPN[0] = index into the level-0 (root) page table
// VPN[1] = index into the level-1 page table
//
// physical address format (34 bits-wide):
//
// PPN[1] PPN[0] offset     <-- field name
//  12     10      12       <-- number of bits
//
// PPN[0] = value from the leaf PTE (must be 0 if is a megapage)
// PPN[1] = value from the leaf PTE
//
// A megapage is a 4MB block on a 4MB boundary.  
// A page is a 4KB block on a 4KB boundary.
//
// A megapage is a level-0 PTE that has either of the R or X flags set to 1 (and PPN[0] = 0)
// A pointer to a level-1 PTE is one with all of the R, W and X flags set to 0
//

struct sv32_table
{
	uint32_t	pte[1024];
};
#define SV32_PTE_V		(1<<0)			// this PTE is valid when 1
#define SV32_PTE_R		(1<<1)			// allow read access when 1
#define SV32_PTE_W		(1<<2)			// allow write access when 1
#define SV32_PTE_X		(1<<3)			// allow exec when 1
#define SV32_PTE_U		(1<<4)			// allow User-mode access
#define SV32_PTE_G		(1<<5)			// globally mapped (same page must be present in all address spaces)
#define SV32_PTE_A		(1<<6)			// accessed flag
#define SV32_PTE_D		(1<<7)			// dirty flag
#define SV32_PTE_RSW	(3<<8)			// reserved for use by supervisor software; 
#define SV32_PTE_PPN0	(0x03ff<<10)	// PPN[0] field
#define SV32_PTE_PPN1	(0x0fff<<20)	// PPN[1] field

// X W R 
// 0 0 0  Pointer to next level of page table.
// 0 0 1  Read-only page.
// 0 1 0    Reserved for future use.
// 0 1 1  Read-write page.
// 1 0 0  Execute-only page.
// 1 0 1  Read-execute page.
// 1 1 0    Reserved for future use.
// 1 1 1  Read-write-execute page.


void sv32_setup();



#endif
