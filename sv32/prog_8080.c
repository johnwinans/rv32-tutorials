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


#include "sifive_test.h"
#include "ecall.h"
#include <stdio.h>
#include <stdint.h>

char buf[1024];
/**
***************************************************************************/
int main()
{

	for (int i=0; i<1024; ++i)
		buf[i] = 'a'+(i%26);			// abcde...abcde...

	ecall(0x111,0x222,0x333,0x444,0x555); // test ecall 

	//asm volatile("ecall");
	//asm volatile("ebreak");

	(*(uint32_t*)0x10000000) = 0xa50;	// this should fail!
	(*(uint32_t*)0x80000000) = 0xa50;	// this should fail!

	// XXX This should page fault when it tries to write to the UART!
	printf("hello from prog_8080.c\n");
	qemu_exit(0);
	return 0;
}
