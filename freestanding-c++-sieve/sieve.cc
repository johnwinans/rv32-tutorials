#include "dbg.h"
#include "hack.h"

#include <stdio.h>
#include <stdlib.h>
#include <set>

using namespace std;

#define ARR_SIZE (200)
#define TEST_ARR_SIZE (100)
#define SEED1 (1)
#define SEED2 (3)
#define LOW (1)
#define HIGH (1000)
#define NO_ITEMS (16)
#define ITEM_W (4)

hack h1;

//char output[1000][16] __attribute__ ((aligned (4096)));	

/**
* Format the contents of the given set.
* @param s The set to print.
*********************************************************************/
void print_primes(const set <int>& s)
{
	DBG("entered\n");
	int i = 0;

	for (auto it=s.begin(); it != s.end(); ++it, ++i)
		printf("%d\n", *it);
//		sprintf(output[i], "%15d", *it);
	DBG("exiting\n");
}

/**
* Fill a set with integers from 2 to n and then apply the
* Sieve of Eratosthenes algorithm to remove all nonprime 
* numbers from the set.
*
* @param s The set to fill in.
* @param n The max/limit value to put into the set before the sieve.
*********************************************************************/
void sieve (set < int >& s, int n)
{
	DBG("entered\n");
	// fill the set
	for (int i=2; i<=n; ++i)
		s.insert(i);

	// sive
	for (int i=2; i<=n; ++i)
		for (int j=i+i; j<=n; j+=i)
			s.erase(j);
	DBG("exiting\n");
}

