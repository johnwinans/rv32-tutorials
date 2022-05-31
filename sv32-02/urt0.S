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


// U-mode startup file

	.section	.init			// force this to come first in the final binary
	.align 2
	.global _start
	.type   _start, @function

_start:

    .cfi_startproc
    .cfi_undefined ra

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

	call    __libc_init_array       // Run global initialization functions

	call    main

	ebreak
#if 0
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
