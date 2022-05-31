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


#include "hexdump.h"

#include <stdio.h>
#include <ctype.h>

void hexdump(const void *p, uint32_t length)
{
    unsigned char*  addr = (unsigned char*)p;
    uint32_t        ch;
    int             i = 0;
    uint32_t        j;
    unsigned char   ascii[20];  /* to hold printable portion of string */

    if (length==0)
        return;

    int startingoffset = ((uint32_t)addr)%16;
    if (startingoffset != 0)
    {
        // pre-padd the dump so it looks purdy
        fprintf(stdout, " %p:", (void*)(((uint32_t)addr)&~0x0f));
        int i;
        for (i=0; i<startingoffset; ++i)
            fprintf(stdout, "   ");
        if (i > 8)
            fprintf(stdout, " ");
    }

    for(j=(uint32_t)addr; j<(((uint32_t)addr)+length); ++j)
    {
        ch = *(unsigned char *)j;
        if ((j % 16) == 0)
        {
            if (j>(uint32_t)addr)
            {
                ascii[i] = '\0';
                fprintf(stdout, " %*s*%s*\n", startingoffset, "", ascii);
                startingoffset = 0;
            }
            fprintf(stdout, " %p:", (unsigned char*)j);
            i = 0;
        }
        fprintf(stdout, "%s%2.2x", (j%8==0&&j%16!=0)?"  ":" ", (unsigned int)(ch & 0xff));

        ascii[i] = ch;
        if ((ascii[i] >= 0x80)||(!isprint(ascii[i])))
            ascii[i] = '.';
        ++i;
    }
    if ((j%16) <= 8 && (j%16)!=0)
        fprintf(stdout, " ");

    while (j%16)
    {
        fprintf(stdout, "   ");
        ++j;
    }
    ascii[i] = '\0';
    fprintf(stdout, " %*s*%s*\n", startingoffset, "", ascii);
}
