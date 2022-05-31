#include "dbg.h"
#include "hack.h"

#include <stdio.h>
#include <stdlib.h>

using namespace std;



hack glob1;
hack glob2;


/**
* Test global constructor/destriuctor
****************************************************************************/
extern "C" int cmain();
int main()
{
	DBG("entered\n");

	DBG("returning\n");
	return 0;
}
