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


#ifndef trap_H
#define trap_H

#include <stdint.h>

#define MIE_SSIE	(1<<1)
#define MIE_MSIE	(1<<3)
#define MIE_STIE	(1<<5)
#define MIE_MTIE	(1<<7)
#define MIE_SEIE	(1<<9)
#define MIE_MEIE	(1<<11)

#define MSTATUS_SIE	(1<<1)
#define MSTATUS_MIE	(1<<3)

/**
* Initialize the mtvec for future traps & IRQs.
***************************************************************************/
void trap_init();

void trap_default_dump();


/**
* Set/enable the traps in the given enable mask.
***************************************************************************/
inline void trap_set_mie(uint32_t enable)
{
    asm volatile("csrrs x0,mie,%0" : : "r"(enable));
}

/**
* Clear/disable the bits in the given disable mask.
***************************************************************************/
inline void trap_clear_mie(uint32_t disable)
{
    asm volatile("csrrc x0,mie,%0" : : "r"(disable));
}

/**
* Set the bits given in the enable mask.
***************************************************************************/
inline void trap_set_mstatus(uint32_t enable)
{
    asm volatile("csrrs x0,mstatus,%0" : : "r"(enable));
}

/**
* Clear the bits in the given disable mask.
***************************************************************************/
inline void trap_clear_mstatus(uint32_t disable)
{
    asm volatile("csrrc x0,mstatus,%0" : : "r"(disable));
}


#endif
