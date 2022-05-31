#    RISC-V rv32 tutorial examples
#
#    Copyright (C) 2021 John Winans
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <https://www.gnu.org/licenses/>.
#

TOP=..
include $(TOP)/Make.rules

LIB_DIR=$(TOP)/lib

CPPFLAGS+=-I$(LIB_DIR)

LDLIBS=
LDLIBS+=-lc
LDLIBS+=-L$(LIB_DIR) -l$(MARCH)
LDLIBS+=-lc -lgcc

CFLAGS+=-O2

LDFLAGS+=-Wl,--no-relax
LDFLAGS+=-Wl,-T,qemu-system-riscv32.ld


# tell the linker to go find the handler for machine software interrupts
LDFLAGS+=-u irq_3

PROGS=prog prog.lst # prog.bin

all:: $(PROGS)

prog: crti.o main.o
	$(LINK.c) -o $@ $^ $(LDLIBS)
	$(SIZE) -A -x $@

#$(LD) -nostdlib -T../lib/qemu-system-riscv32.ld -o $@ $^ -L../lib -\( -lrv32ima -\) -lc -lgcc

clean::
	rm -f $(PROGS) *.o *.s *.lst *.bin *.srec
