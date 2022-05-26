/******************************************************************************/
#ifndef __D_H_
#define __D_H_
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

//smg
#define DM XBYTE[0XFEFF]//p2.0
#define WM XBYTE[0XFDFF]

extern u8 sbuf[8];

//8255key
#define PA XBYTE[0XE3FF]
#define PB XBYTE[0XE7FF]
#define PC XBYTE[0XEBFF]
#define CN XBYTE[0XEFFF]

extern u16 mkv,tkv;

//lcd
sbit cs2=P2^1;
sbit cs1=P2^0;
sbit e=P3^2;
sbit di=P3^3;
extern bit flcd;
extern u8x vm[8][128];

//adc,dac
#define DAC XBYTE[0XDFFF]

extern u8 vadc7,vadc6;
extern bit fdac;

//zl
sbit pri=P3^4;
sbit fx=P3^5;
sbit k=P3^6;
#define stop() (pri=0,fzl=0)

extern u8 zlm,kong;
extern u16 zln,time;
extern bit fzl;


//bjdj
sbit cp=P5^5;
sbit dir=P4^5;
sbit rl=P4^1;

extern u16 cc,pp;
extern u8 th1,tl1;
extern bit fbj;
/******************************************************************************/
/*         F u n c t i o n      D e f i n e                                   */
/******************************************************************************/
void init();
void dms(u16 ms);
void dus(u16 us);
void sd();
void ks();
void lcdclr(u8 d);
void flush();
void initlcd(u8 d);
void wzf(int x,int y,u8 w,u8 h,u8 *zm,bit c);
void wlhz(int x,int y,u8 w,u8 h,u8 *str,bit c);
void wlzf(int x,int y,u8 w,u8 h,u8 *str,bit c);
void dp(int x,int y,bit c);
void shu(int x,int y1,int y2,bit c);
void heng(int y,int x1,int x2,bit c);
void dl(int x1,int y1,int x2,int y2,bit c);
void initadc();
void shukong();
void zlrun(u8 m,u8 num,bit d);
void initbj();
void to(u8 mm,u8 s);
void inituart();
void putc(u8 ch);
void putstr(u8 *str);
void pwm(u8 ch,u8 v);
/******************************************************************************/
#endif
/******************************************************************************/
