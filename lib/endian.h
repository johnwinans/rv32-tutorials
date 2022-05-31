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


#ifndef endian_H
#define endian_H

#include <stdint.h>

// This is modeled after the same functions on Linux

uint16_t htobe16(uint16_t host_16bits);
uint16_t be16toh(uint16_t big_endian_16bits);

inline uint16_t htole16(uint16_t host_16bits) { return host_16bits; }
inline uint16_t le16toh(uint16_t little_endian_16bits) { return little_endian_16bits; }

uint32_t htobe32(uint32_t host_32bits);
uint32_t be32toh(uint32_t big_endian_32bits);

inline uint32_t htole32(uint32_t host_32bits) { return host_32bits; }
inline uint32_t le32toh(uint32_t little_endian_32bits) { return little_endian_32bits; }

uint64_t htobe64(uint64_t host_64bits);
uint64_t be64toh(uint64_t big_endian_64bits);
inline uint64_t htole64(uint64_t host_64bits) { return host_64bits; }
inline uint64_t le64toh(uint64_t little_endian_64bits) { return little_endian_64bits; }


#endif
