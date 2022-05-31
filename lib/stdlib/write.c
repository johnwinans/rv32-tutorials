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


#include "ns16550.h"

/**
* This function should satify the caller by simply returning len
* indicating that the write has succeeded as requested in spite
* of the fact that the data is simply ignored/discarded.
*
* @return len
*****************************************************************/
int _write(int file, char *ptr, int len) 
{
#if 1
	// qemu-system-riscv32 -machine virt has a 16550 at address 0x10000000
	//volatile char *thr = (volatile char *)0x10000000;
	for (int i=0; i<len; ++i)
		ns16550_tx(ptr[i]);
		//*thr = ptr[i];
#endif
	return len;
}
