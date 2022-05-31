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


#ifndef dbg_H
#define dbg_H

#include <stdio.h>

#define ENABLE_DBG

#ifdef ENABLE_DBG
#define DBG(...) do { printf("*** %s %d %s(): ", __FILE__, __LINE__, __FUNCTION__); printf(__VA_ARGS__); fflush(stdout); } while(0)
#else
#define DBG(...) do { } while(0)
#endif

#endif
