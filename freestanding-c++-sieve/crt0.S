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


	//.text
    .section    .text.crt0,"ax",@progbits
	.align 2
	.global _start
	.type   _start, @function

_start:

.cfi_startproc			// tell the debugger this is a function
.cfi_undefined ra		// tell the debugger that the ra register is not viable here


.option push
.option norelax
	# Initialize global pointer
	la		gp,__global_pointer$
.option pop

	// Set up a viable stack pointer using a label provided by the linker
	la		x2,__ram_end__

	# Clear the bss segment
	la      a0,__bss_start__
	la      a1,__bss_end__

clear_bss:
	bgeu	a0,a1,finish_bss
	sb		x0,0(a0)
	addi	a0,a0,1
	beq		x0,x0,clear_bss
finish_bss:

#if 1
	.weak __tls_base
	la tp, __tls_base
#endif


	// prepare to call destrctors etc when exit is invoked
	la		a0,__libc_fini_array
	call	atexit

	call    __libc_init_array       // Run global initialization functions
	//call	callConstructors		// call any global constructors

#if 1
	li		a0,0xffffffff
	csrc	mie,a0		// disable all IRQs
	csrc	mip,a0		// clear any pending IRQs
	csrc	satp,a0		// shut off virtual memory
	csrc	mideleg,a0	// delegate nothing
	csrc	medeleg,a0	// delegate nothing

#endif




    li      a0,0        # a0 = argc = 0
    li      a1,0        # a1 = argv = NULL
    li      a2,0        # a2 = envp = NULL

	call    main
#if 0
	//call    cmain

	// save the return code from main()
	mv		s0,a0

	//call	callDestructors		// call any global destructors
	//call    __libc_fini_array       // Run global initialization functions

	li		a0,1
	la		a1,xitmsg1
	la		a2,8
	call	_write

	// this propagates the return value in a0 from main() into exit()
	mv		a0,s0
#endif

wtf:
	call	exit		// why does this not do the global destructors?

wtf2:
	// we should not ever get here !!

	li		a0,1
	la		a1,xitmsg2
	la		a2,6
	call	_write

	// Disable ALL irqs and traps
    csrw	mie,x0
	// Spin while waiting for an interrupt that can never arrive. (Seize the processor.)
spin:
	wfi							// This will prevent qemu from using 100% of a host core!
	j		spin

.cfi_endproc


xitmsg1:
	.asciz	"EXITING\n"
xitmsg2:
	.asciz	"ERROR: NOT SUPPOSED TO BE HERE!!\n"

	.end
