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


#include <errno.h>
#undef errno
extern int errno;

/**
* This library does not support any file I/O of any kind.
*
* @return -1 (error codition.)
*****************************************************************/
int _open(const char *name, int flags, int mode) 
{ 
	errno = ENFILE;		// The system-wide limit (0) on total open files has been reached.
	return -1; 
}
