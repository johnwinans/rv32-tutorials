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


#ifndef plic_H
#define plic_H

#include <stdint.h>

// From the qemu 'info qtree' command when running with 8 harts:
//
//   dev: riscv.sifive.plic, id ""
//    gpio-in "" 127
//    hart-config = "MS,MS,MS,MS,MS,MS,MS,MS"
//    num-sources = 127 (0x7f)
//    num-priorities = 7 (0x7)
//    priority-base = 4 (0x4)
//    pending-base = 4096 (0x1000)
//    enable-base = 8192 (0x2000)
//    enable-stride = 128 (0x80)
//    context-base = 2097152 (0x200000)
//    context-stride = 4096 (0x1000)
//    aperture-size = 67108864 (0x4000000)
//    mmio 000000000c000000/0000000004000000
//
// The MS stuff in the 'hart-config' line means that the plic has
// 2 contexts for each of 8 harts.  One for machine & one for supervisor.
//
// Therefore:
// context 0 = hart 0, machine
// context 1 = hart 0, supervisor
// context 2 = hart 1, machine
// context 3 = hart 1, supervisor
// ...
//
// The PLIC looks like this in the device tree:
//
//	interrupt-controller@c000000 {
//		phandle = <0x11>;
//		riscv,ndev = <0x35>;
//		reg = <0x00 0xc000000 0x00 0x4000000>;
//		interrupts-extended = <0x10 0x0b 0x10 0x09 0x0e 0x0b 0x0e 0x09 0x0c 0x0b 0x0c 0x09 0x0a 0x0b 0x0a 0x09 0x08 0x0b 0x08 0x09 0x06 0x0b 0x06 0x09 0x04 0x0b 0x04 0x09 0x02 0x0b 0x02 0x09>;
//		interrupt-controller;
//		compatible = "riscv,plic0";
//		#interrupt-cells = <0x01>;
//		#address-cells = <0x00>;
//	};
//
// The interrupts-extended value has a pair for each of 16 PLIC contexts that mean:
//		0x10 0x0b = route to IRQ 0x0b of phandle 0x10 (the interrupt-controller in hart 0)
//		0x10 0x09 = route to IRQ 0x09 of phandle 0x10 
//		0x0e 0x0b = route to IRQ 0x0b of phandle 0x0e (the interrupt-controller in hart 1)
//  	...
//
// IRQ 0x09 = Supervisor External IRQ
// IRQ 0x0b = Machine External IRQ
//
// Therefore the PLIC should be configurable to generate either a supervisor 
// or machine interrupt on any hart.



#define PLIC_NUM_CONTEXT		(0x4000-64)	// 16k-64
#define PLIC_NUM_SOURCE			(0x400)		// 1k

#define PLIC_NUM_QEMU_HARTS		(8)
#define PLIC_NUM_QEMU_SOURCE	(32)					// number of IRQs actually implemented on qemu??
#define PLIC_NUM_QEMU_CONTEXT	(PLIC_NUM_QEMU_HARTS*2)	// number of actual contexts in MS config

struct plic_prio_claim_t
{
	uint32_t	prio_threshold;		///< block all pending IRQs <= threashold 
	uint32_t	claim_complete;
	uint32_t	reserved[PLIC_NUM_SOURCE-2];
};
struct plic_t
{
	uint32_t	priority[PLIC_NUM_SOURCE];			///< 0=disable, 1=0, 7=high
	uint32_t	pending[PLIC_NUM_SOURCE/32];		///< one bit per source, 1=pending
	uint32_t	reserved[992];
	uint32_t	enable[PLIC_NUM_CONTEXT][PLIC_NUM_SOURCE/32];	///< enable each source for each context
	struct plic_prio_claim_t prio_claim[PLIC_NUM_CONTEXT];
};

#define PLIC	((volatile struct plic_t*)(0xc000000))


// The UART device tree entry indicates that it is connected to 
// PLIC source 0x0a:
//
//	uart@10000000 {
//		interrupts = <0x0a>;
//		interrupt-parent = <0x03>;
//		clock-frequency = <0x384000>;
//		reg = <0x00 0x10000000 0x00 0x100>;
//		compatible = "ns16550a";
//	};


#define PLIC_IRQ_UART	(0x0a)


void plic_init();
void plic_set_enable(uint32_t enable, uint32_t hartid);
void plic_dump();

#endif
