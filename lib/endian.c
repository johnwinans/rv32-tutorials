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


#include "endian.h"

uint16_t htobe16(uint16_t host_16bits)
{
	uint16_t i = ((host_16bits&0x0ff)<<8) | ((host_16bits&0xff00)>>8);
	return i;
}

uint16_t be16toh(uint16_t big_endian_16bits)
{
	uint16_t i = ((big_endian_16bits&0x0ff)<<8) | ((big_endian_16bits&0xff00)>>8);
	return i;
}

uint32_t htobe32(uint32_t host_32bits)
{
	uint32_t i = ((uint32_t)htobe16(host_32bits&0x0ffff)<<16) | htobe16((host_32bits>>16)&0x0ffff);
	return i;
}

uint32_t be32toh(uint32_t big_endian_32bits)
{
	uint32_t i = ((uint32_t)be16toh(big_endian_32bits&0x0ffff)<<16) | be16toh((big_endian_32bits>>16)&0x0ffff);
	return i;
}

uint64_t htobe64(uint64_t host_64bits)
{
	uint64_t i = ((uint64_t)htobe32(host_64bits&0x0ffffffffULL)<<32) | htobe32((host_64bits>>32)&0x0ffffffffULL);
	return i;
}

uint64_t be64toh(uint64_t big_endian_64bits)
{
	uint64_t i = ((uint64_t)htobe32(big_endian_64bits&0x0ffffffffULL)<<32) | htobe32((big_endian_64bits>>32)&0x0ffffffffULL);
	return i;
}
