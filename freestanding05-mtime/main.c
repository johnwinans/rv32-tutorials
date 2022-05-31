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


#include "clint.h"
#include "sifive_test.h"

#include <stdio.h>

/**
***************************************************************************/
int main()
{
	printf("clint: %p\n", &CLINT0);
	printf("    msip: %p\n", CLINT0.msip);
	printf("    mtimecmp: %p\n", CLINT0.mtimecmp);
	printf("    mtime: %p\n", &CLINT0.mtime);

	while(1)
	{
		uint64_t mtime = clint_get_mtime();
		uint32_t hh = mtime/10000000/60/60;
		uint32_t mm = (mtime/10000000/60)%60;
		uint32_t ss = (mtime/10000000)%60;
		uint32_t nn = (mtime/10)%1000000;
		printf("time=0x%16.16llx, %lu:%02lu:%02lu.%6.6lu\n", mtime, hh, mm, ss, nn);
	}

	return 0;
}
