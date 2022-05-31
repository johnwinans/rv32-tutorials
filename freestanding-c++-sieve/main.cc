#include "dbg.h"
#include "sieve.h"

#include <stdio.h>
#include <stdlib.h>
#include <set>

using namespace std;

/**
* Perform the sieve of Eratosthenes and print the prime numbers 
* from 2 to 1000 into the output array.
*********************************************************************/
extern "C" int cmain();

int main()
{
	DBG("entered\n");
    uint32_t my_mhartid;    // purposly named different than the csr to reduce confusion
    asm volatile("csrrs %0,mhartid,x0" : "=r"(my_mhartid) );    // read the mhartid register
    uint32_t my_misa;
    asm volatile("csrrs %0,misa,x0" : "=r"(my_misa) );

    printf("mhartid=0x%8.8x, misa=0x%8.8x\n", (unsigned int)my_mhartid, (unsigned int)my_misa);

    printf("rv32");
    for (uint8_t i=0; i<26; ++i)
    {
        if (my_misa&1)
            printf("%c", 'a'+i);
        my_misa>>=1;
    }
    printf("\n");

	set <int> s;

	DBG("constructed the set\n");

	sieve(s, 1000);
	print_primes(s);

	DBG("returning\n");
	return 0;
}
