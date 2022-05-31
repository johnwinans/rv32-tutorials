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

#include <stdio.h>

#define MCAUSE_IRQ_SUPERVISOR_SOFTWARE	(1)
#define MCAUSE_IRQ_MACHINE_SOFTWARE		(3)
#define MCAUSE_IRQ_SUPERVISOR_TIMER		(5)
#define MCAUSE_IRQ_MACHINE_TIMER		(7)
#define MCAUSE_IRQ_SUPERVISOR_EXT		(9)
#define MCAUSE_IRQ_MACHINE_EXT			(11)

void trap_default_exception_handler(void);
void trap_default_interrupt_handler(void);
void trap_register_interrupt_handler(uint32_t i, void(*func)(void));
void trap_register_exception_handler(uint32_t i, void(*func)(void));
void trap_init();

#endif
