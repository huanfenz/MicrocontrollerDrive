#define _ISR_C_
#include "isr.h"
#include "d.h"

u16 cnt;
u8 vdac;
bit fdac;
void tt0() interrupt 1
{
	TH0=0XF8;
	TL0=0XCC;
	cnt++;
	if(cnt==500)
	{
		//1s
		cnt=0;
	}
	if(cnt%5==0)
	{
		//10ms
		ks();
	}
	if(cnt%50==0)
	{
		//100ms
		EX1=IE1=1;
		if(fdac)
		{
			fdac=0;
			DAC=vdac;
		}
		if(fzl)
		{
			shukong();
			time++;
			if(zlm==2)zln--;
			if(zln==0)stop();
		}
	}
	sd();
}

void int1() interrupt 2
{
	flush();
	EX1=IE1=0;
}

void adisr() interrupt 5
{
	u8 td;
	td=ADC_CONTR&0x07;//ȡADC_CONTR�ĵ�3λ������CHS2��CHS1��CHS0��ֵ��ȷ��ͨ��
	switch (td)
	{
		//case :	break;
		default:	break;
		case 7:vadc7=ADC_RES;ADC_CONTR=0xe8+6;break;//��ת���������vadc7������6ͨ����ת��
		case 6:vadc6=ADC_RES;ADC_CONTR=0xe8+7;break;//��ת���������vadc6������7ͨ����ת��
		//Result,�Ĵ�����ΪADC_RES��λ,ADC_RESL��λ��ȡ8λ�Ľ��ֻ��Ҫʹ��ADC_RES�е����ݡ�
		//0xe8��1110 1000,CHS����0��Ȼ������ʲôͨ����+����
	}
}

void tt1() interrupt 3
{
	TR1=0;
	TH1=th1;
	TL1=tl1;
	if(rl==1)
	{
		dir=0;
		cc=65536-3050;
	}
	cp=~cp;
	if(dir)cc--;
	else cc++;
	if(cc!=pp)TR1=1;
	else fbj=0;
}
