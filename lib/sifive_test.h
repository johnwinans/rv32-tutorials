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


#ifndef sifive_test_H
#define sifive_test_H

#include <stdint.h>


/**
* @file Registers implemented in the sifive_test virtual device that can 
*		be used to halt the qemu simulator.
*
* To halt qemu from guest code: `SIFIVE_TEST_REG = SIFIVE_TEST_REG_PASS;`
*
* To halt qemu with a non-zero exit code: `SIFIVE_TEST_REG = SIFIVE_TEST_REG_FAIL|(123<<16);`
*
* @note Interestingly, due to the implementation, `SIFIVE_TEST_REG = SIFIVE_TEST_REG_FAIL;`
*		will exit with a zero... thus representing a "pass" of sorts.
****************************************************************************/

#define SIFIVE_TEST_REG (*((volatile uint32_t *)0x100000))

#define SIFIVE_TEST_REG_FAIL	((uint32_t)0x3333)		///< Use like this: SIFIVE_TEST_REG_FAIL|(123<<16) to exit(123)
#define SIFIVE_TEST_REG_PASS	((uint32_t)0x5555)		///< This will cause qsim to exit(0)
#define SIFIVE_TEST_REG_RESET	((uint32_t)0x7777)		///< This will reset & restart the simulation

// XXX the reset appears to be deferred somehow.  
// XXX If write reset then fail, the reset will not happen and the fail will.
// XXX If you want to reset qemu, do this: 
//       SIFIVE_TEST_REG = SIFIVE_TEST_REG_RESET; 
//       while(1) ;


/**
* Call this to terminate qemu with the given exit code.
****************************************************************************/
inline __attribute__((always_inline)) void qemu_exit(uint16_t code)
{
	if (code !=0)
		SIFIVE_TEST_REG = SIFIVE_TEST_REG_FAIL|(code<<16);	///< Yeah, I know.. but keep in the spirit
	else
		SIFIVE_TEST_REG = SIFIVE_TEST_REG_PASS;
}

#endif
