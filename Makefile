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



SUBDIRS=\
	lib\
	freestanding01\
	freestanding02\
	freestanding03\
	freestanding04-qemu-exit\
	freestanding05-mtime\
	freestanding06-panic\
	freestanding07-mtime-irq\
	freestanding08-mret\
	freestanding-c++-dtor\
	freestanding-c++-sieve\
	irq\
	msu\
	sv32\
	sv32-02

#	irqn\

TOP=.
include $(TOP)/Make.rules
