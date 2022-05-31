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


#include "virtio.h"

#include <stdint.h>
#include <stdio.h>


/*
    Reset the device.
    Set the ACKNOWLEDGE status bit: the guest OS has noticed the device.
    Set the DRIVER status bit: the guest OS knows how to drive the device.
    Read device feature bits, and write the subset of feature bits understood by the OS and driver to the device. During this step the driver MAY read (but MUST NOT write) the device-specific configuration fields to check that it can support the device before accepting it.
    Set the FEATURES_OK status bit. The driver MUST NOT accept new feature bits after this step.
    Re-read device status to ensure the FEATURES_OK bit is still set: otherwise, the device does not support our subset of features and the device is unusable.
    Perform device-specific setup, including discovery of virtqueues for the device, optional per-bus setup, reading and possibly writing the device’s virtio configuration space, and population of virtqueues.
    Set the DRIVER_OK status bit. At this point the device is “live”.
*/

void virtio_dump(void *p)
{
	uint32_t *pv = (uint32_t*)p;

	for (int i=0; i<4; ++i)
		printf("%p: %08lx\n", &pv[i], pv[i]);
}
