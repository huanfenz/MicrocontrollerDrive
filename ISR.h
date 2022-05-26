/******************************************************************************/
#ifndef __ISR_H_
#define __ISR_H_
/******************************************************************************/
#include "m.h"
#ifdef _TEST_C_
#define GLOBAL
#else
#define GLOBAL extern
#endif
/*
example:
GLOBAL char variable
GLOBAL bit flag
*/
extern u16 cnt;
extern u8 vdac;
extern bit fdac;
/******************************************************************************/
#endif
/******************************************************************************/
