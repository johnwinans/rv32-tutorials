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


#ifndef lock_H
#define lock_H

#include <stdint.h>

extern volatile uint32_t stdio_lock;

/**
* Perform an atomic-swap to set the given lock to 1
***************************************************************************/
inline void lock(volatile uint32_t *plock) __attribute__((always_inline));
inline void lock(volatile uint32_t *plock)
{
    uint32_t t = 1;
    while(t)
        asm volatile("amoswap.w.aq %0,%0,(%1)" : "+r"(t) : "r"(plock) );
}

/**
***************************************************************************/
inline void unlock(volatile uint32_t *plock) __attribute__((always_inline));
inline void unlock(volatile uint32_t *plock)
{
    asm volatile("amoswap.w.rl x0,x0,(%0)" : : "r"(plock) );
}

#endif
