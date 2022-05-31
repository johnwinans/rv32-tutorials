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


#include <stdint.h>
/**
* Adjust the brk pointer up or down as requested.
*
* The initial brk address is set to _end (the end of the BSS).
* Any requests to adjust the brk will be performed without any
* error checking.
*
* @param delta The number of bytes to raise or lower the brk.
*
* @return The address that brk was set to before adjusting it by
*	delta.  Note that when delta is positive, this will return the 
*	address of the newly allocated region of memory.
*
* @bug It is possible that an errant program could call this and
*	reduce the brk such that it points below _end or increase the
*	brk to the point that it overlaps the stack.
*****************************************************************/
char *_sbrk (int delta) 
{
	extern char _end[];
	static char *end_of_data = 0; //_end;
#if 1
	if (end_of_data==0)
		end_of_data = (char*)((((uint32_t)_end)+0x0f)&0xfffffff0);		// align to 16
#endif
	char *ptr = end_of_data;
	end_of_data += delta;
	return ptr;
}
