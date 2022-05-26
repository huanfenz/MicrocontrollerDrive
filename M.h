/******************************************************************************/
#ifndef __M_H_
#define __M_H_
/******************************************************************************/
#include "STC15F2K60S2.h"
#include "stdio.h"
#include "math.h"
#include "ctype.h"
#include "string.h"
#include "stdlib.h"
#include "absacc.h"
#include "intrins.h"
#include "stdarg.h"
#include "setjmp.h"
typedef unsigned char u8;
typedef unsigned int u16;
typedef u8 xdata u8x;
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

/******************************************************************************/
#endif
/******************************************************************************/
