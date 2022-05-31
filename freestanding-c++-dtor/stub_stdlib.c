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

#include <sys/stat.h>
#include <errno.h>


#include <errno.h>
#undef errno
extern int errno;


/**
* This will execute an ebreak instruction and assume that the
* CPU will halt or simulation environment will terminate.
*****************************************************************/
void _exit(int i)
{
	asm volatile (" ebreak ");
}

/**
* It is assumed that there is exactly only process running and that
* it does not support signals.  Therefore calling this is effectively
* illegal and will therefore execute an ebreak instruction.
*****************************************************************/
void _kill(int pid) 
{
#if 1
	asm volatile (" ebreak ");
#else
	return;						// arguably, this might also be acceptable
#endif
}

/**
* This returns the process ID of the runnung program.
* This library assumes that there is only process that
* can ever run.
*
* @return 1
*****************************************************************/
int _getpid(void) 
{
  return 1;
}
 

/**
* This library does not support any file I/O of any kind.
*
* @return -1 Indicating that file could not be closed.
*****************************************************************/
int _close(int file) 
{
	errno = EBADF;
	return -1; 
}
 
/**
* This library does not support any file I/O of any kind.
* This call will return a status indicating that the file
* in question is a character device.
*
* @return 0 Indicating that the call has succeeded.
*****************************************************************/
int _fstat(int file, struct stat *st) 
{
	st->st_mode = S_IFCHR;
	return 0;
}
 
/**
* This library does not support any file I/O of any kind.
*
* @return 1 Indicating that file is a tty device (a terminal.)
*****************************************************************/
int _isatty(int file) 
{
	return 1;
}
 
/**
* This library does not support any file I/O of any kind.
*
* @return 0 Indicating that the request has succeeded.
*****************************************************************/
int _lseek(int file, int ptr, int dir) 
{ 
	return 0; 
}
 
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
 
/**
* This library does not support any file I/O of any kind.
*
* @return EOF.
*****************************************************************/
int _read(int file, char *ptr, int len) 
{
	return 0;
}

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
	static char *end_of_data = _end;

	char *ptr = end_of_data;
	end_of_data += delta;
	return ptr;
}
