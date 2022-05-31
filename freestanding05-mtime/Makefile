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
LDLIBS+=-L$(LIB_DIR)
LDLIBS+=-Wl,--start-group
LDLIBS+=-lc
LDLIBS+=-l$(MARCH)
LDLIBS+=-Wl,--end-group
LDLIBS+=-lgcc


CFLAGS+=-O2

#LDFLAGS+=-Wl,--no-relax
LDFLAGS+=-Wl,-T,qemu-system-riscv32.ld

PROGS=prog prog.lst # prog.bin

all:: $(PROGS)

prog: crt0.o main.o
	$(LINK.c) -o $@ $^ $(LDLIBS)
	$(SIZE) -A -x $@

clean::
	rm -f $(PROGS) *.o *.s *.lst *.bin *.srec
