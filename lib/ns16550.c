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

#include <stdio.h>


/**
* Dump the state of the UART.
***************************************************************************/
void ns16550_dump()
{
	uint32_t mip;
	uint32_t mie;
	asm volatile("csrrs %0,mip,x0" : "=r"(mip) );
	asm volatile("csrrs %0,mie,x0" : "=r"(mie) );
	printf("ns16550_dump(): mip=0x%8.8lx, mie=0x%8.8lx, IER=0x%2.2x, IIR=0x%2.2x, FCR=0x%2.2x, LCR=0x%2.2x, LSR=0x%2.2x\n", mip, mie, NS16550_IER, NS16550_IIR, NS16550_FCR, NS16550_LCR, NS16550_LSR);
}

/**
* PLIC IRQ handler for the UART.
***************************************************************************/
void plic_10()
{
	if (ns16550_rx_ready())
	{
		uint8_t ch = ns16550_rx();
		printf("plic_10()/ns16550: got character '%c'\n", ch);
	}
}


/**
* Enable/disable IRQs from the UART.
***************************************************************************/
void ns16550_enable_irq(uint8_t enable)
{
    //NS16550_IER = (enable? NS16550_IER_ERBFI|NS16550_IER_ETBEI : 0);
    NS16550_IER = (enable? NS16550_IER_ERBFI : 0);
}
