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


#include "hexdump.h"
#include "ns16550.h"
#include "csr.h"		// to use the fancy inlined csr access functions

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
***************************************************************************/
int main()
{
#if 1
	uint32_t my_mhartid = csr_read_mhartid();
	uint32_t my_misa = csr_read_misa();
#else
	uint32_t my_mhartid;	// purposly named different than the csr to reduce confusion 
	asm volatile("csrrs %0,mhartid,x0" : "=r"(my_mhartid) );	// read the mhartid register
	uint32_t my_misa;
	asm volatile("csrrs %0,misa,x0" : "=r"(my_misa) );	
#endif

	printf("mhartid=0x%8.8x, misa=0x%8.8x\n", (unsigned int)my_mhartid, (unsigned int)my_misa);

	printf("rv32");
	for (uint8_t i=0; i<26; ++i)
	{
		if (my_misa&1)
			printf("%c", 'a'+i);
		my_misa>>=1;
	}
	printf("\n");

	extern uint8_t __ram_origin__[];
	extern uint8_t __ram_end__[];
	printf("__ram_origin=%p\n", __ram_origin__);
	printf("__ram_end=%p\n", __ram_end__);

	printf("Top of the stack:\n");
	hexdump(__ram_end__-0x400, 0x400);			// pointer arith is intuitive here because sizeof(*__ram_end__) == 1

	while(1)
	{
		ns16550_tx('.');						// print a dot
		for (uint32_t j=0; j<100000000; ++j)	// waste some time
			;
	}

	// we never get here

	return 0;
}
