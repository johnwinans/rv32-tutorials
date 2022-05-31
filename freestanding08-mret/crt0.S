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


	.text
	.global _start
	.type   _start, @function
	.align 2

_start:		// This has to match the entry address in the ld script!!

    .cfi_startproc
    .cfi_undefined ra


#if 1
	// test mret to jump over the ebreak instruction
	//li		x15,0x1800		// target mode = M
	li		x15,0x0800		// target mode = S 
	csrrs	x0,mstatus,x15
	la		x15,go
	csrrw   x0,mepc,x15
	mret

	ebreak
go:
#endif


	li		x5,0x10000000		// x5 = @ XS16550 transmitter holding register
2:
	la		x6,hello			// x6 = @ of null-terminated string to print

	// check if nothing to print
	lb		x7,0(x6)			// x7 = next byte to print from the string
	beq		x0,x7,2f			// if the character is \0 then we are done

	// have something to print
1:
	sb		x7,0(x5)			// else write it to the THR 
	addi	x6,x6,1				// advance string pointer to the next character to print
	lb		x7,0(x6)			// x7 = next byte to print from the string
	bne		x0,x7,1b			// if the character is \0 then we are done

#if 0	// XXX
	addi	x5,x5,0x100			// point to the next UART
	li		x6,0x10000400		// are we past the last one yet?
	bne		x5,x6,2b			// if x5 != 0x10000400 then go back & print to wanother UART 
#endif


2:
	// disable ALL irqs and traps
    //csrw	mie,x0				// not needed in this app (and will trap if running in S mode!!)

	// Spin while waiting for an interrupt that can never arrive. (Seize the processor.)
1:
	wfi							// This will prevent qemu from using 100% of a host core!
	j		1b

	.cfi_endproc

	.section .rodata			// want in a data section so not end up in an exec only page!
	//.data
hello:
	.asciz	"Hello World!\n"	// asciz strings have a null-character appended to it:
								// 48 65 6c 6c 6f 20 57 6f 72 6c 64 21 0a 00

	.end
