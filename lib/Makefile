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

SUBDIRS=stdlib

TOP=..
include $(TOP)/Make.rules


#CFLAGS+=-O
#CFLAGS+=-O2


LIB=lib$(MARCH).a

OBJS=\
	panic.o \
	trap.o \
	msi.o \
	hexdump.o \
	lock.o \
	clint.o \
	ns16550.o \
	plic.o \
	virtio.o \
	ecall.o \
	endian.o

STDLIB_OBJS=\
	stdlib/close.o \
	stdlib/exit.o \
	stdlib/fstat.o \
	stdlib/getpid.o \
	stdlib/isatty.o \
	stdlib/kill.o \
	stdlib/lseek.o \
	stdlib/open.o \
	stdlib/read.o \
	stdlib/sbrk.o \
	stdlib/write.o

	

all:: $(LIB)

$(LIB): $(OBJS) $(STDLIB_OBJS)
	rm -f $@
	$(AR) r $@ $^
	$(RANLIB) $@

clean::
	rm -f $(OBJS) *.o *.a *.s *lst
