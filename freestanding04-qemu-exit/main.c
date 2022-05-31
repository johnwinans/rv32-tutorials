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

#include <stdio.h>

#define SIFIVE_TEST_REG	(*((volatile uint32_t *)0x100000))

/**
***************************************************************************/
int main()
{
	printf("application started\n");

//	SIFIVE_TEST_REG = SIFIVE_TEST_REG_PASS;
//	SIFIVE_TEST_REG = SIFIVE_TEST_REG_FAIL|(123<<16);
//	SIFIVE_TEST_REG = SIFIVE_TEST_REG_RESET; while(1) ;  // for some reason, reset takes multiple cycles.

	qemu_exit(68);

	printf("If you're close enough to read this... It didn't work. :-(\n");
	return 0;
}
