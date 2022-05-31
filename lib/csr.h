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


#ifndef csr_H
#define csr_H

#include <stdint.h>


#define MIE_SSIE	(1<<1)
#define MIE_MSIE	(1<<3)
#define MIE_STIE	(1<<5)
#define MIE_MTIE	(1<<7)
#define MIE_SEIE	(1<<9)
#define MIE_MEIE	(1<<11)

#define MSTATUS_SIE		(1<<1)
#define MSTATUS_MIE		(1<<3)
#define MSTATUS_SPIE	(1<<5)
#define MSTATUS_UBE		(1<<6)
#define MSTATUS_MPIE	(1<<7)
#define MSTATUS_SPP		(1<<8)

#define SSTATUS_SPP		MSTATUS_SPP
#define SSTATUS_SPP_U	MSTATUS_SPP_U


#define MSTATUS_MPP		(3<<11)
#define MSTATUS_MPP_U	(0<<11)
#define MSTATUS_MPP_S	(1<<11)
#define MSTATUS_MPP_M	(3<<11)

#define MSTATUS_SUM		(1<<18)
#define SSTATUS_SUM		MSTATUS_SUM


// XXX it would be easier to implement these using a gcc-specific ({uint32_t x; asm(); x; }) statement expression

#define csr_read(target_u32, csr)	asm volatile("csrrs %0," #csr ",x0" : "=r"(target_u32) )
#define csr_write(csr, source_u32)	asm volatile("csrrw x0," #csr ",%0" : : "r"(source_u32) )
#define csr_swap(target_u32, csr, source_u32)	asm volatile("csrrw %0," #csr ",%1" : "=r"(target_u32) : "r"(source_u32) )
#define csr_set(csr, source_u32)	asm volatile("csrrs x0," #csr ",%0" : : "r"(source_u32) )
#define csr_clear(csr, source_u32)	asm volatile("csrrc x0," #csr ",%0" : : "r"(source_u32) )


#define csr_funcs(csr) \
	inline __attribute__((always_inline)) uint32_t csr_read_##csr() { uint32_t u; csr_read(u, csr); return u; } \
	inline __attribute__((always_inline)) void csr_write_##csr(uint32_t v)  { csr_write(csr, v); } \
	inline __attribute__((always_inline)) void csr_set_##csr(uint32_t on) { csr_set(csr, on); } \
	inline __attribute__((always_inline)) void csr_clear_##csr(uint32_t off)  { csr_clear(csr, off); } \
	inline __attribute__((always_inline)) uint32_t csr_swap_##csr(uint32_t v) { uint32_t u; csr_swap(u, csr, v); return u; }

csr_funcs(ustatus)
csr_funcs(uie)
csr_funcs(utvec)
csr_funcs(uscratch)
csr_funcs(uepc)
csr_funcs(ucause)
csr_funcs(utval)
csr_funcs(uip)
csr_funcs(fflags)
csr_funcs(frm)
csr_funcs(fcsr)
//csr_funcs(cycle)
//csr_funcs(time)
//csr_funcs(instret)
csr_funcs(hpmcounter3)
csr_funcs(hpmcounter4)
csr_funcs(hpmcounter5)
csr_funcs(hpmcounter6)
csr_funcs(hpmcounter7)
csr_funcs(hpmcounter8)
csr_funcs(hpmcounter9)
csr_funcs(hpmcounter10)
csr_funcs(hpmcounter11)
csr_funcs(hpmcounter12)
csr_funcs(hpmcounter13)
csr_funcs(hpmcounter14)
csr_funcs(hpmcounter15)
csr_funcs(hpmcounter16)
csr_funcs(hpmcounter17)
csr_funcs(hpmcounter18)
csr_funcs(hpmcounter19)
csr_funcs(hpmcounter20)
csr_funcs(hpmcounter21)
csr_funcs(hpmcounter22)
csr_funcs(hpmcounter23)
csr_funcs(hpmcounter24)
csr_funcs(hpmcounter25)
csr_funcs(hpmcounter26)
csr_funcs(hpmcounter27)
csr_funcs(hpmcounter28)
csr_funcs(hpmcounter29)
csr_funcs(hpmcounter30)
csr_funcs(hpmcounter31)

//csr_funcs(cycleh)
//csr_funcs(timeh)
//csr_funcs(instreth)
csr_funcs(hpmcounter3h)
csr_funcs(hpmcounter4h)
csr_funcs(hpmcounter5h)
csr_funcs(hpmcounter6h)
csr_funcs(hpmcounter7h)
csr_funcs(hpmcounter8h)
csr_funcs(hpmcounter9h)
csr_funcs(hpmcounter10h)
csr_funcs(hpmcounter11h)
csr_funcs(hpmcounter12h)
csr_funcs(hpmcounter13h)
csr_funcs(hpmcounter14h)
csr_funcs(hpmcounter15h)
csr_funcs(hpmcounter16h)
csr_funcs(hpmcounter17h)
csr_funcs(hpmcounter18h)
csr_funcs(hpmcounter19h)
csr_funcs(hpmcounter20h)
csr_funcs(hpmcounter21h)
csr_funcs(hpmcounter22h)
csr_funcs(hpmcounter23h)
csr_funcs(hpmcounter24h)
csr_funcs(hpmcounter25h)
csr_funcs(hpmcounter26h)
csr_funcs(hpmcounter27h)
csr_funcs(hpmcounter28h)
csr_funcs(hpmcounter29h)
csr_funcs(hpmcounter30h)
csr_funcs(hpmcounter31h)

csr_funcs(sstatus)
csr_funcs(sedeleg)
csr_funcs(sideleg)
csr_funcs(sie)
csr_funcs(stvec)
csr_funcs(scounteren)
csr_funcs(sscratch)
csr_funcs(sepc)
csr_funcs(scause)
csr_funcs(stval)
csr_funcs(sip)

csr_funcs(satp)
csr_funcs(scontext)

csr_funcs(hstatus)
csr_funcs(hedeleg)
csr_funcs(hideleg)
csr_funcs(hie)
csr_funcs(hcounteren)
csr_funcs(hgeie)
csr_funcs(htval)
csr_funcs(hip)
csr_funcs(hvip)
csr_funcs(htinst)
csr_funcs(hgeip)

csr_funcs(hgatp)

csr_funcs(hcontext)

csr_funcs(htimedelta)
csr_funcs(htimedeltah)

csr_funcs(vsstatus)
csr_funcs(vsie)
csr_funcs(vstvec)
csr_funcs(vsscratch)
csr_funcs(vsepc)
csr_funcs(vscause)
csr_funcs(vstval)
csr_funcs(vsip)
csr_funcs(vsatp)

csr_funcs(mvendorid)
csr_funcs(marchid)
csr_funcs(mimpid)
csr_funcs(mhartid)

csr_funcs(mstatus)
csr_funcs(misa)
csr_funcs(medeleg)
csr_funcs(mideleg)
csr_funcs(mie)
csr_funcs(mtvec)
csr_funcs(mcounteren)
csr_funcs(mstatush)
csr_funcs(mscratch)
csr_funcs(mepc)
csr_funcs(mcause)
csr_funcs(mtval)
csr_funcs(mip)
csr_funcs(mtinst)
csr_funcs(mtval2)

csr_funcs(pmpcfg0)
csr_funcs(pmpcfg1)
csr_funcs(pmpcfg2)
csr_funcs(pmpcfg3)
csr_funcs(pmpcfg4)
csr_funcs(pmpcfg5)
csr_funcs(pmpcfg6)
csr_funcs(pmpcfg7)
csr_funcs(pmpcfg8)
csr_funcs(pmpcfg9)
csr_funcs(pmpcfg10)
csr_funcs(pmpcfg11)
csr_funcs(pmpcfg12)
csr_funcs(pmpcfg13)
csr_funcs(pmpcfg14)
csr_funcs(pmpcfg15)
csr_funcs(pmpaddr0)
csr_funcs(pmpaddr1)
csr_funcs(pmpaddr2)
csr_funcs(pmpaddr3)
csr_funcs(pmpaddr4)
csr_funcs(pmpaddr5)
csr_funcs(pmpaddr6)
csr_funcs(pmpaddr7)
csr_funcs(pmpaddr8)
csr_funcs(pmpaddr9)
csr_funcs(pmpaddr10)
csr_funcs(pmpaddr11)
csr_funcs(pmpaddr12)
csr_funcs(pmpaddr13)
csr_funcs(pmpaddr14)
csr_funcs(pmpaddr15)
csr_funcs(pmpaddr16)
csr_funcs(pmpaddr17)
csr_funcs(pmpaddr18)
csr_funcs(pmpaddr19)
csr_funcs(pmpaddr20)
csr_funcs(pmpaddr21)
csr_funcs(pmpaddr22)
csr_funcs(pmpaddr23)
csr_funcs(pmpaddr24)
csr_funcs(pmpaddr25)
csr_funcs(pmpaddr26)
csr_funcs(pmpaddr27)
csr_funcs(pmpaddr28)
csr_funcs(pmpaddr29)
csr_funcs(pmpaddr30)
csr_funcs(pmpaddr31)
csr_funcs(pmpaddr32)
csr_funcs(pmpaddr33)
csr_funcs(pmpaddr34)
csr_funcs(pmpaddr35)
csr_funcs(pmpaddr36)
csr_funcs(pmpaddr37)
csr_funcs(pmpaddr38)
csr_funcs(pmpaddr39)
csr_funcs(pmpaddr40)
csr_funcs(pmpaddr41)
csr_funcs(pmpaddr42)
csr_funcs(pmpaddr43)
csr_funcs(pmpaddr44)
csr_funcs(pmpaddr45)
csr_funcs(pmpaddr46)
csr_funcs(pmpaddr47)
csr_funcs(pmpaddr48)
csr_funcs(pmpaddr49)
csr_funcs(pmpaddr50)
csr_funcs(pmpaddr51)
csr_funcs(pmpaddr52)
csr_funcs(pmpaddr53)
csr_funcs(pmpaddr54)
csr_funcs(pmpaddr55)
csr_funcs(pmpaddr56)
csr_funcs(pmpaddr57)
csr_funcs(pmpaddr58)
csr_funcs(pmpaddr59)
csr_funcs(pmpaddr60)
csr_funcs(pmpaddr61)
csr_funcs(pmpaddr62)
csr_funcs(pmpaddr63)

csr_funcs(mcycle)
csr_funcs(minstret)
csr_funcs(mhpmcounter3)
csr_funcs(mhpmcounter4)
csr_funcs(mhpmcounter5)
csr_funcs(mhpmcounter6)
csr_funcs(mhpmcounter7)
csr_funcs(mhpmcounter8)
csr_funcs(mhpmcounter9)
csr_funcs(mhpmcounter10)
csr_funcs(mhpmcounter11)
csr_funcs(mhpmcounter12)
csr_funcs(mhpmcounter13)
csr_funcs(mhpmcounter14)
csr_funcs(mhpmcounter15)
csr_funcs(mhpmcounter16)
csr_funcs(mhpmcounter17)
csr_funcs(mhpmcounter18)
csr_funcs(mhpmcounter19)
csr_funcs(mhpmcounter20)
csr_funcs(mhpmcounter21)
csr_funcs(mhpmcounter22)
csr_funcs(mhpmcounter23)
csr_funcs(mhpmcounter24)
csr_funcs(mhpmcounter25)
csr_funcs(mhpmcounter26)
csr_funcs(mhpmcounter27)
csr_funcs(mhpmcounter28)
csr_funcs(mhpmcounter29)
csr_funcs(mhpmcounter30)
csr_funcs(mhpmcounter31)
csr_funcs(mcycleh)
csr_funcs(minstreth)
csr_funcs(mhpmcounter3h)
csr_funcs(mhpmcounter4h)
csr_funcs(mhpmcounter5h)
csr_funcs(mhpmcounter6h)
csr_funcs(mhpmcounter7h)
csr_funcs(mhpmcounter8h)
csr_funcs(mhpmcounter9h)
csr_funcs(mhpmcounter10h)
csr_funcs(mhpmcounter11h)
csr_funcs(mhpmcounter12h)
csr_funcs(mhpmcounter13h)
csr_funcs(mhpmcounter14h)
csr_funcs(mhpmcounter15h)
csr_funcs(mhpmcounter16h)
csr_funcs(mhpmcounter17h)
csr_funcs(mhpmcounter18h)
csr_funcs(mhpmcounter19h)
csr_funcs(mhpmcounter20h)
csr_funcs(mhpmcounter21h)
csr_funcs(mhpmcounter22h)
csr_funcs(mhpmcounter23h)
csr_funcs(mhpmcounter24h)
csr_funcs(mhpmcounter25h)
csr_funcs(mhpmcounter26h)
csr_funcs(mhpmcounter27h)
csr_funcs(mhpmcounter28h)
csr_funcs(mhpmcounter29h)
csr_funcs(mhpmcounter30h)
csr_funcs(mhpmcounter31h)

csr_funcs(mcountinhibit)
csr_funcs(mhpmevent3)
csr_funcs(mhpmevent4)
csr_funcs(mhpmevent5)
csr_funcs(mhpmevent6)
csr_funcs(mhpmevent7)
csr_funcs(mhpmevent8)
csr_funcs(mhpmevent9)
csr_funcs(mhpmevent10)
csr_funcs(mhpmevent11)
csr_funcs(mhpmevent12)
csr_funcs(mhpmevent13)
csr_funcs(mhpmevent14)
csr_funcs(mhpmevent15)
csr_funcs(mhpmevent16)
csr_funcs(mhpmevent17)
csr_funcs(mhpmevent18)
csr_funcs(mhpmevent19)
csr_funcs(mhpmevent20)
csr_funcs(mhpmevent21)
csr_funcs(mhpmevent22)
csr_funcs(mhpmevent23)
csr_funcs(mhpmevent24)
csr_funcs(mhpmevent25)
csr_funcs(mhpmevent26)
csr_funcs(mhpmevent27)
csr_funcs(mhpmevent28)
csr_funcs(mhpmevent29)
csr_funcs(mhpmevent30)
csr_funcs(mhpmevent31)

csr_funcs(tselect)
csr_funcs(tdata1)
csr_funcs(tdata2)
csr_funcs(tdata3)
csr_funcs(mcontext)

csr_funcs(dcsr)
csr_funcs(dpc)
csr_funcs(dscratch0)
csr_funcs(dscratch1)


// special case instructions so that RV32 can read 64-bit timers and counters
#define csr_rd64(csr)										\
inline __attribute__((always_inline)) uint64_t csr_rd##csr()\
{															\
	union													\
	{														\
		uint64_t	v64;									\
		struct												\
		{													\
			uint32_t	lo;									\
			uint32_t	hi;									\
		} v32;												\
	} v;													\
	uint32_t	i;											\
															\
	do														\
	{														\
		asm volatile("rd" #csr "h %0": "=r"(v.v32.hi) );		\
		asm volatile("rd" #csr " %0"	: "=r"(v.v32.lo) );		\
		asm volatile("rd" #csr "h %0": "=r"(i) );			\
	}														\
    while (i != v.v32.hi);									\
    return v.v64;											\
}

csr_rd64(time)
csr_rd64(cycle)
csr_rd64(instret)


#endif
