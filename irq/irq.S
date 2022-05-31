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


	.text
	.align 6		// On the FE310-G002: In vectored interrupt mode, BASE must be 64-byte aligned.
	.global irq_vector_table

irq_vector_table:
	j		irq_0
	j		irq_1
	j		irq_2
	j		irq_3
	j		irq_4
	j		irq_5
	j		irq_6
	j		irq_7
	j		irq_8
	j		irq_9
	j		irq_10
	j		irq_11
	j		irq_12
	j		irq_13
	j		irq_14
	j		irq_15

	.end
