#define _D_C_
#include "d.h"
#include "zm.h"

// 延迟毫秒
void dms(u16 ms)
{
	u16 i;
	while (ms--)
	{
		i=300;
		while(i--);
	}
}

// 延迟微秒
void dus(u16 us)
{
	while(us--);
}

// 中断初始化
void init()
{
	EA=ET0=ET1=1;
	TMOD=0X11;
	TH0=0XF8;
	TL0=0XCC;

	IT1=1;
	PT0=1;
	PT1=1;

	TR0=1;
}

// PS：数码管为共阳极数码管

// 数码管编码
u8 code dm[]={
	0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xff,	// 0到9，全灭
	0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f,0xff,
};

// 数码管
u8 sbuf[8]={
	1,2,3,4,5,6,0,0,
};
void sd()
{
	static u8 i;
	DM=0xff;
	WM=~(0x80>>i%8);
	DM=dm[sbuf[i++%8]];
}

// 矩阵按键
u16 mkv=0xffff,tkv=0xffff;
void ks()
{
	static u8 pkv=0xff,t;
	u8 ckv=0xff;
	CN=0x9a;
	PC=0xf0;
	ckv=PC;

	CN=0x93;
	PC=0x0f;
	ckv|=PC;
	if(ckv!=pkv)
	{
		tkv=mkv=pkv<<8|ckv;
		pkv=ckv;
		t=0;
	}
	else
	{
		t++;
		if(t>=100&&t%25==0)
		{
			if(t==250)t=100;
			mkv=tkv;
		}
	}
}

// LCD(12864)
u8x vm[8][128]={
	0,
};
bit flcd=0;
// 待补充
void wr(bit dc,u8 dt)
{
	ET0=0;
	di=dc;dus(1);
	P0=dt;dus(1);
	e=1;dus(1);
	e=0;dus(1);
	ET0=1;
}
// 清空
void lcdclr(u8 d)
{
	u8 i,j;
	for(i=0;i<8;i++)
	for(j=0;j<128;j++)
	vm[i][j]=d;
	flcd=1;
}
// 刷新
void flush()
{
	u8 i,j;
	
	if(flcd==0)return;
	flcd=0;
	for(i=0;i<8;i++)
	{
		cs1=1;cs2=0;
		wr(0,0xb8+i);
		wr(0,0x40);
		for(j=0;j<64;j++)wr(1,vm[i][j]);

		cs1=0;cs2=1;
		wr(0,0xb8+i);
		wr(0,0x40);
		for(j=64;j<128;j++)wr(1,vm[i][j]);
	}
	cs1=cs2=0;
}
// 初始化
void initlcd(u8 d)
{
	lcdclr(d);
	wr(0,0x3f);
	wr(0,0xc0);
}
// 待补充
void wb(int x,int y,u8 d,bit c)
{
	u8 pg;
	if(y<-7||y>63)return;
	pg=y/8;
	if(c)d=~d;
	if(y%8==0)vm[pg][x]=d;
	else if(y<0)
	{
		vm[0][x]&=0xff<<(8+y);
		vm[0][x]|=d>>(-y);
	}
	else if(y>56)
	{
		vm[7][x]&=0xff>>(64-y);
		vm[7][x]|=d<<(y-56);
	}
	else if(y%8)
	{
		vm[pg][x]&=0xff>>(8-y%8);
		vm[pg][x]|=d<<(y%8);
		vm[pg+1][x]&=0xff<<(y%8);
		vm[pg+1][x]|=d>>(8-y%8);
	}
	flcd=1;
}
// 绘制字符
void wzf(int x,int y,u8 w,u8 h,u8 *zm,bit c)
{
	int i,j,k;
	if(zm==NULL)return;
	k=h/8+(h%8?1:0);
	for(i=0;i<k;i++)
	for(j=0;j<w;j++)
	wb(x+j,y+8*i,zm[j+w*i],c);
}
// 待补充
u8 *fi(u8 *str)
{
	u8 i;
	for(i=0;i<sizeof(hz)/34;i++)
	{
		if(str[0]==hz[i].index[0]&&str[1]==hz[i].index[1])
		return hz[i].msk;
	}
	return NULL;
}
// 绘制汉字
void wlhz(int x,int y,u8 w,u8 h,u8 *str,bit c)
{
	u8 i=0;
	while (str[i*2]!='\0')
	{
		wzf(x+w*i,y,w,h,fi(&str[i*2]),c);
		i++;
	}
}
// 绘制字符
void wlzf(int x,int y,u8 w,u8 h,u8 *str,bit c)
{
	u8 i=0;
	while (str[i]!='\0')
	{
		wzf(x+w*i,y,w,h,asc[str[i]-32],c);
		i++;
	}
}

// 画点
void dp(int x,int y,bit c)
{
	u8 pg;
	if(x>127||x>63||x<0||y<0)return;
	pg=y/8;
	if(c)vm[pg][x]|=1<<(y%8);
	else vm[pg][x]&=~(1<<(y%8));
	flcd=1;
}

// 画竖线
void shu(int x,int y1,int y2,bit c)
{
	int i;
	for(i=y1;i<=y2;i++)
	dp(x,i,c);
}

// 画横线
void heng(int y,int x1,int x2,bit c)
{
	int i;
	for(i=x1;i<=x2;i++)
	dp(i,y,c);
}

//void dl(int x1,int y1,int x2,int y2,bit c)
//{
//	int i,d,x,y,dx,dy,tmp,sub;
//	char sy,sx;
//	x=x1;y=y1;
//	if(x2>x1)
//	{
//		dx=x2-x1;sx=1;
//	}
//	else
//	{
//		dx=x1-x2;sx=-1;
//	}
//	if(y2>y1)
//	{
//		dy=y2-y1;sy=1;
//	}
//	else
//	{
//		dy=y1-y2;sy=-1;
//	}
//	if(dy>dx)
//	{
//		tmp=dx;dx=dy;dy=tmp;d=1;
//	}
//	else d=0;
//	sub=(dy<<1)-dx;
//	dp(x,y,c);
//	for(i=0;i<dx;i++)
//	{
//		if(d)y+=sy;
//		else x+=sx;
//		if(sub>0)
//		{
//			if(d)x+=sx;
//			else y+=sy;
//			sub+=(dy-dx)<<1;
//		}
//		else sub+=dy<<1;
//		dp(x,y,c);
//	}
////输入程序代码
//}

//void dc(int x,int y,int r,bit c)
//{
//	int idata a,b,di;
//	a=0;b=r;di=3-2*r;
//	while (a<=b)
//	{
//		dp(x-a,y-b,c);
//		dp(x+a,y-b,c);
//		dp(x-a,y+b,c);
//		dp(x+a,y+b,c);
//		dp(x-b,y-a,c);
//		dp(x+b,y-a,c);
//		dp(x-b,y+a,c);
//		dp(x+b,y+a,c);
//		a++;
//		if(di<0)di+=4*a+6;
//		else
//		{
//			di+=10+4*(a-b);
//			b--;
//		}
//		dp(x+a,y+b,c);
//	}
////输入程序代码	
//}

//void setstart(u8 screen,char num)
//{
//	cs1=screen/10;
//	cs2=screen%10;
//	if(num<0)wr(0,0xc0+64+num%64);
//	else wr(0,0xc0+num%64);
//	cs1=cs2=0;
////输入程序代码
//}

// 模数转换（STC15F2K60S2带的功能）
u8 vadc6,vadc7;
void initadc()
{
	P1ASF=0xc0;
	/*P1 Analog Super Function
	1100 0000，即P1.6，P1.7用作模拟功能A/D使用*/

	
	EA=EADC=1;//A/D转化中断允许位

	/*Control ADC控制
	ADC_POWER	SPEED1	SPEED0	ADC_FLAG	ADC_START	CHS2	CHS1	CHS0
	1			1		1		0			1			1		1		1
	CHS2，CHS1，CHS0选择输入通道
	000 P1^0 -> 111 P1^7*/

	ADC_CONTR=0Xe8+7;
}

// 直流电机
u8 zlm,kong;
u16 zln,time;
bit fzl;
// 直流点击数孔
void shukong()
{
	static bit pk=1;
	bit ck;
	k=1;
	ck=k;
	if(ck==0&&pk==1)
	{
		kong++;
		if(zlm==1)zln--;
	}
	pk=ck;
}

// 直流电机转
void zlrun(u8 m,u8 num,bit d)
{
	zlm=m;
	zln=num;
	fx=d;
	pri=1;
	fzl=1;
}

// 步进电机
u16 cc,pp;
u8 th1,tl1;
bit fbj;
// 初始化（回0，需要调试）
void initbj()
{	
	fbj=1;
	dir=1;
	th1=(65536-900000/275/10)/256;
	tl1=(65536-900000/275/10)%256;
	cc=pp=0;
	TR1=1;
}
// 步进电机到某个刻度
void to(u8 mm,u8 s)
{
	if(mm==cc/275)return;
	if(fbj)TR1=0;
	fbj=1;
	th1=(65536-900000/275/s)/256;
	tl1=(65536-900000/275/s)%256;
	if(mm<cc/275)dir=1;
	else dir=0;
	pp=mm*275;
	TR1=1;
}
// 串口初始化
void inituart()
{
	S2CON=0X10;
	T2H=(65536-230400/9600)/256;
	T2L=(65536-230400/9600)%256;
	AUXR|=0X10;
	IE2|=0X01;
}
// 输出字符
void putc(u8 ch)
{
	S2CON&=0xfd;
	S2BUF=ch;
	while(!(S2CON&0x02));
	S2CON&=0xfd;
}
// 复数字符串
void putstr(u8 *str)
{
	u8 i=0;
	while(str[i]!='\0')
		putc(str[i++]);
	putc('\n');
}
// 脉宽调制（STC15F2K60S2带的功能）
void pwm(u8 ch,u8 v)
{
	CMOD=0;
	AUXR1&=0XCF;
	AUXR1|=0X20;
	CH=CL=0;
	switch (ch)
	{
		//case :	break;
		default:	break;
		case 0:CCAPM0=0x42;PCA_PWM0=0;CCAP0H=CCAP0L=v*256/100;break;
		case 1:CCAPM1=0x42;PCA_PWM1=0;CCAP1H=CCAP1L=v*256/100;break;
		case 2:CCAPM2=0x42;PCA_PWM2=0;CCAP2H=CCAP2L=v*256/100;break;
	}
	CR=1;
}

