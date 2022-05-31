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

#CFLAGS+=-Q --help=optimizers


# verbose assembler listings
#CFLAGS+=-Wa,-alh,-L
#CFLAGS+=-fverbose-asm

 
LDFLAGS+=-Wl,-Bstatic

#LDFLAGS+=-Wl,--print-map
#LDFLAGS+=-Wl,--cref
#LDFLAGS_system+=-Wl,-v

#LDFLAGS+=-Wl,--no-relax
#LDFLAGS+=-Wl,--noinhibit-exec

#LDFLAGS_system+=-Wl,-T,qemu-system-riscv32.ld
#LDFLAGS_system+=-Wl,-T,new.ld
#LDFLAGS_system+=-Wl,-T,riscv32-machine.ld
LDFLAGS_system+=-Wl,-T,riscv32-machine-mem.ld
#LDFLAGS_system+=-Wl,-Ttext=0x80000000

#LDFLAGS_system+=-Wl,--print-map
#LDFLAGS_system+=-Wl,--cref

#LDFLAGS_app+=-Wl,-T,qemu-system-riscv32.ld
LDFLAGS_app+=-Wl,-T,riscv32-user.ld
#LDFLAGS_app+=-Wl,-Ttext=0x90000000

PROGS=prog prog.lst prog_8080 prog_8080.lst prog_8080.bin

all:: $(PROGS)

prog: crt0.o main.o sv32.o
	$(LINK.c) $(LDFLAGS) $(LDFLAGS_system) -o $@ $^ $(LDLIBS)
	$(SIZE) -A -x $@

# User progs need custom linker files for memory address assignments
#prog_8080: crt0.o prog_8080.o
prog_8080: crt0.o prog_8080.o
	$(LINK.c) $(LDFLAGS) $(LDFLAGS_app) -o $@ $^ $(LDLIBS)
	$(SIZE) -A -x $@

clean::
	rm -f $(PROGS) *.o *.s *.lst *.bin *.srec
