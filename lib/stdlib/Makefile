#
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



TOP=../..
include $(TOP)/Make.rules

CFLAGS+=-O2

CPPFLAGS+=-I..

# For some seriously verbose assembly output during compilation...
#CFLAGS+=-Wa,-adhln -g -fverbose-asm


OBJS=\
	close.o \
	exit.o \
	fstat.o \
	getpid.o \
	isatty.o \
	kill.o \
	lseek.o \
	open.o \
	read.o \
	sbrk.o \
	write.o

all:: $(OBJS)

clean::
	rm -f *.o *.s *.lst
