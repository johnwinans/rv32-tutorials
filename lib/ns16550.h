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


#ifndef ns16550_H
#define ns16550_H

#include <stdint.h>

// The UART on the virt machine will generate IRQ 0x0a on the PLIC


// qemu-system-riscv32 dtb values
#define NS16550_THR	(*((volatile uint8_t *)0x10000000))
#define NS16550_RBR	(*((volatile uint8_t *)0x10000000))
#define NS16550_IER	(*((volatile uint8_t *)0x10000001))
#define NS16550_IIR	(*((volatile uint8_t *)0x10000002))
#define NS16550_FCR	(*((volatile uint8_t *)0x10000002))
#define NS16550_LCR	(*((volatile uint8_t *)0x10000003))
#define NS16550_MCR	(*((volatile uint8_t *)0x10000004))
#define NS16550_LSR	(*((volatile uint8_t *)0x10000005))
#define NS16550_MSR	(*((volatile uint8_t *)0x10000006))
#define NS16550_SCR	(*((volatile uint8_t *)0x10000007))

#define NS16550_LSR_DR		(1<<0)
#define NS16550_LSR_THRE	(1<<5)
#define NS16550_LSR_TEMT	(1<<6)

#define NS16550_IER_ERBFI	(1<<0)
#define NS16550_IER_ETBEI	(1<<1)

/**
* Wait for the TX FIFO to have room for a byte and send it.
***************************************************************************/
inline void ns16550_tx(uint8_t ch)
{
	// be careful about order of operations here...
	while((NS16550_LSR & NS16550_LSR_THRE) == 0) 
		;
	NS16550_THR = ch;
}

/**
* Wait for the TX FIFO to be completely drained.
***************************************************************************/
inline void ns16550_tx_flush()
{
	while((NS16550_LSR & NS16550_LSR_TEMT) == 0)
		;
}


/**
* @return Nonzero value if there is data ready to be read from the RX FIFO.
***************************************************************************/
inline uint8_t ns16550_rx_ready()
{
	return NS16550_LSR & NS16550_LSR_DR;
}

/**
* Wait for the RX FIFO to have a character in it and return it.
***************************************************************************/
inline uint8_t ns16550_rx()
{
	while(ns16550_rx_ready() == 0) 
		;
	return NS16550_RBR;
}

void ns16550_dump();
void ns16550_enable_irq(uint8_t enable);


#endif
