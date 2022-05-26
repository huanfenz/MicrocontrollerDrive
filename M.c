#define _M_C_
#include "m.h"
#include "d.h"
#include "isr.h"

void main()
{
	init();
	inituart();
	initadc();
	initlcd(0);
}
