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


// M-mode startup file

	.section	.init			// force this to come first in the final binary
	.align 2
	.global _start
	.type   _start, @function

_start:

	.cfi_startproc
	.cfi_undefined ra

	// On qemu, the startup process starts by executing code at 0x1000 that 
	// will load a0 with the mhartid and a1 with the address of a devicetree 
	// structure.

	// Initially, send ALL M-mode and S-mode traps to panic and halt the machine.
	csrrw	x0,mie,x0		// mie = 0 (disable all interrupts)
	csrrw	x0,mip,x0		// mip = 0 (clear all pending interrupts)
	la		t1,mtpanic
	csrrw	x0,mtvec,t1
	la		t1,stpanic
	csrrw	x0,stvec,t1		// S-mode panic will ebreak into M-mode and panic there


	// By default, all traps at any privilege level are handled in machine mode.
	// In systems with S-mode, the medeleg and mideleg registers must exist, and 
	// setting a bit in medeleg or mideleg will delegate the corresponding trap, 
	// when occurring in S-mode or U-mode, to the S-mode trap handler.
	// See: 'The RISC-V Instruction Set Manual, Volume II: Privileged Architecture, 
	// Document Version 1.12-draft,' Editors Andrew Waterman and Krste Asanovic, 
	// RISC-V Foundation, June 2019.


	li		t1,(1<<1)|(1<<5)|(1<<9)
	csrrw	x0,mideleg,t1	// delegate S-mode software, timer and external IRQs to S-mode

	li		t1,(1<<8)
	csrrw	x0,medeleg,t1	// delegate U-mode ecall to S-mode


	// enter S-mode and start the O/S

	la		t1,s_mode_start
	csrrw	x0,mepc,t1

	li		t1,(3<<11)		// mpp mask
	csrrc	x0,mstatus,t1	// mpp = 0
	li		t1,(1<<11)		// S-mode
	csrrs	x0,mstatus,t1	// mpp = S-mode

	mret					// warp into S-mode and start running s_mode_start() 


#if 0
.option push
.option norelax
	la		gp,__global_pointer$	// Initialize global pointer
.option pop

	// Set up a viable stack pointer using a label provided by the linker
	la		x2,__stack_top__

	# Clear the bss segment
	la      a0,__bss_start
	la      a1,__BSS_END__

clear_bss:
	bgeu	a0,a1,finish_bss
	sb		x0,0(a0)
	addi	a0,a0,1
	beq		x0,x0,clear_bss
finish_bss:

//.option push
//.option norelax
	call    __libc_init_array       // Run global initialization functions
//.option pop

	call    main

	// Disable ALL irqs and traps
    csrw	mie,x0
	// disable all delegation

	// Spin while waiting for an interrupt that can never arrive. (Seize the processor.)
spin:
	wfi								// This will prevent qemu from using 100% of a host core
	j		spin
#endif

	.cfi_endproc

	.end
