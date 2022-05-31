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


	.section	.init
	.align 2
	.global _start
	.type   _start, @function

_start:

	.cfi_startproc
	.cfi_undefined ra

.option push
.option norelax
	la      gp,__global_pointer$
.option pop
	
	la		x2,__ram_end__-0x1000	// back off for a trap save area

#if 0
	// qemu will zero all the RAM before running anything

	// XXX This will fail if ALL the harts do not progress at the same rate!
	// XXX And this DOES fail randomly when running on qemu.
	// XXX A better solution would be to implement a barrier with machine
	// XXX software interrupts.

	// Make SURE that the init_done flag is zero before ANY hart starts waiting!
	la		x5,init_done
	amoswap.w.aqrl	x0,x0,(x5)		// ALL the harts will do this!
#endif

	csrrs	x5,mhartid,x0			// load the hart ID into a temp reg

	// Allocate individual stacks for each hart
	slli    x5,x5,20				// temp = hartid * 0x00100000; (1MB per stack)
	sub     x2,x2,x5				// sp = sp-temp

	bne		x5,x0,wait_hart0		// wait for hart 0 to initialize everything

    // hart 0: clear the bss segment
    la      x5,__bss_start
    la      x6,__BSS_END__
    bgeu    x5,x6,finish_bss		// if nothing to clear then skip it
clear_bss:
    sb      x0,0(x5)
    addi    x5,x5,1
    blt     x5,x6,clear_bss			// if not yet done, go back and do more
finish_bss:

	call    __libc_init_array       // Run global initialization functions

	// hart 0 will set the init_done flag to 1
	la		x5,init_done
	li		x6,1
	amoswap.w.aqrl	x0,x6,(x5)		// set the init_done flag (sequentially consistent store)
	//sw		x6,0(x5)			// naive: this might be a problem due to memory access ordering

	j		run_main


wait_hart0:
	la		x5,init_done
wait_hart0_loop:
	lr.w.aqrl		x6,(x5)			// Load the init_done flag (sequentially consistent load)
	//lw			x6,0(x5)		// The priv-spec suggests that this is would be OK
	//amoor.w.aqrl	x6,x0,(x5)		// Very conservative but less efficient than lw (do we care at this point?)

	beq		x6,x0,wait_hart0_loop	// If the init_done is zero then keep checking


run_main:
	call	main


done:
	wfi
	j		done

	.cfi_endproc

	.global init_done
	.lcomm	init_done,4				// A one-word flag to indicate that hart 0 is done initializing

	.end
