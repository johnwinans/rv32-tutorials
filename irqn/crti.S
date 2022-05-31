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

	csrr	x5,mhartid				// load the hart ID into a temp reg

	// Allocate individual stacks for each hart
	la		x2,__ram_end__-0x1000	// back off for a trap save area (see trap.c)
	slli    x5,x5,20				// temp = hartid * 0x00100000; (1MB per stack)
	sub     x2,x2,x5				// sp = sp-temp

	bne		x5,x0,wait_init_done	// wait for hart 0 to initialize everything



    // hart 0: clear the bss segment
	// (they all could do it BUT would have to all complete before any calls main)
    la      x5,__bss_start
    la      x6,__BSS_END__
    bgeu    x5,x6,finish_bss		// if nothing to clear then skip it
clear_bss:
    sb      x0,0(x5)
    addi    x5,x5,1
    blt     x5,x6,clear_bss			// if not yet done, go back and do more
finish_bss:

	call    __libc_init_array       // run global initialization functions


	// XXX slopy hardcodes on 0x2000000 and 8 in here
	// hart 0 releases all the /other/ harts when the address space has been set up
	li		x10,0x2000004			// x10 = clint base address = &clint.msip[1]

	// XXX figure out how to determine the number of harts in the system here!
	// (in theory we could do this until we encounter a write exception on the msip and then have learned)
	li		x11,0x2000000+(8*4)		// x11 = total number of harts * 4
	li		x12,1					// x13 = clint msip msi mask 
signal_loop:
	sw		x12,0(x10)
	addi	x10,x10,4				// point to the next msip register
	bne		x11,x10,signal_loop

	j		run_main



wait_init_done:
	// set direct mode trap handler address to: msi_irq
	la		x10,msi_irq
	csrrw	x0,mtvec,x10

	// enable MSI (machine software interrupts)
	li		x10,0x08				// MSIE bit mask
	csrrs	x0,mie,x10				// enable software interrupts: mie |= 0x08

wait_hart0:
	li		x10,0x08				// MIE bit mask
	csrrs	x0,mstatus,x10			// enable interrupts: mstatus |= 0x08 
wait_hart0_spin:
	wfi
	j		wait_hart0_spin
	

msi_irq:
	// if mcause == IRQ_3 then is a machine software IRQ else an impossible situation
	csrr	x10,mcause				// read the mcause CSR
	li		x11,0x80000003			// if mcause==(IRQ|3) then we got an MSI
	bne		x10,x11,wait_hart0		// not what we are looking for. Keep waiting.

	// need to shut off the clint.msip bit here else will keep getting an MSI
	csrr	x5,mhartid				// get my hart ID 
	slli	x5,x5,2					// x5 = x5 * 4
	la		x10,0x2000000			// x10 = &clint.msip[0]
	add		x10,x10,x5				// x10 += mhartid*4
	sw		x0,0(x10)				// clint.msip[mhartid] = 0


run_main:
	// note: IRQs will have been disabled in mstatus at this point

	li		x10,-1
	csrrc   x0,mie,x10				// disable all types of interrupts
	//csrrc   x0,mip,x10			// clear any pending IRQs (note that mip can be WARL)

	// if main does not re-initialize the mtvec, and an IRQ arrives, go to stop
	// (...just don't leave mtvec pointing at msi_irq!)
	la		x10,stop
	csrrw	x0,mtvec,x10


	call	main


stop:
	// disable interrupts & halt the hart
	li		x10,0x0a				// (MIE|SIE) bit mask
	csrrc	x0,mstatus,x10			// shut off MI and SI IRQs: mstatus &= ~0x0a
done:
	wfi								// wait forever
	j		done

	.cfi_endproc

	.end
