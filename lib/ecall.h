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


#ifndef ecall_H
#define ecall_H

#if 0
uint32_t ecall(uint32_t syscall, ...);
#else
inline __attribute__((always_inline)) uint32_t ecall(uint32_t syscall, ...)
{
	uint32_t rc;
	//asm volatile("fence iorw,iorw");
	asm volatile("ecall" : "=r"(rc) : "r"(syscall) : "memory");
	//asm volatile("fence iorw,iorw");
	return rc;
}
#endif

#endif
