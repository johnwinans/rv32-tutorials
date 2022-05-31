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


#ifndef clint_H
#define clint_H

#include <stdint.h>

///
/// CLINT_MTIME_HZ is the timebase-frequency from the qemu dtb
#define CLINT_MTIME_HZ	((uint64_t)0x989680)

///
/// The address of the mtimecmp memory-mapped register
#define CLINT_MTIMECMP (*((volatile uint64_t*)(0x2004000)))

///
/// The address of the mtime memory-mapped register
#define CLINT_MTIME (*((volatile uint64_t*)(0x200bff8)))

void clint_set_mtimecmp_period(uint64_t dt);

#endif
