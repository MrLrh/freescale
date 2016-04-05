#include "Include.h"

void udelay(int us)
{
  int udelay_i;
  for(udelay_i=0;udelay_i<us; udelay_i++) 
  {
    __asm nop; __asm nop; __asm nop; __asm nop;       //_asm nop 1个时钟周期     1us
		__asm nop; __asm nop; __asm nop; __asm nop;
		__asm nop; __asm nop; __asm nop; __asm nop;
		__asm nop; __asm nop; __asm nop; __asm nop;
		__asm nop; __asm nop; __asm nop; __asm nop;
		__asm nop; __asm nop; __asm nop; __asm nop;
		__asm nop; __asm nop; __asm nop; __asm nop;
		__asm nop; __asm nop; __asm nop; __asm nop;
		__asm nop; __asm nop; __asm nop; __asm nop;
		__asm nop; __asm nop; __asm nop; __asm nop;
		__asm nop; __asm nop; __asm nop; __asm nop;
		__asm nop; __asm nop; __asm nop; __asm nop;
		__asm nop; __asm nop; __asm nop; __asm nop;
		__asm nop; __asm nop; __asm nop; __asm nop;
		__asm nop; __asm nop; __asm nop; __asm nop; 
  }
}
/* CPU delay 250ns at 60M bus clock */
void Cpu_Delay250ns(void) 
{
  __asm(nop); __asm(nop); __asm(nop); __asm(nop);
  __asm(nop); __asm(nop); __asm(nop); __asm(nop);
  __asm(nop); __asm(nop); __asm(nop); __asm(nop);
  __asm(nop); __asm(nop); __asm(nop);   
}                                     
void delay(word  xms)          //1ms延时程序
{
  int i;
  for(i=xms*1000;i>0;i--) 
  {
    __asm nop; __asm nop; __asm nop; __asm nop;       //_asm nop 1个时钟周期     1us
		__asm nop; __asm nop; __asm nop; __asm nop;
		__asm nop; __asm nop; __asm nop; __asm nop;
		__asm nop; __asm nop; __asm nop; __asm nop;
	 	__asm nop; __asm nop; __asm nop; __asm nop;
		__asm nop; __asm nop; __asm nop; __asm nop;
		__asm nop; __asm nop; __asm nop; __asm nop;
		__asm nop; __asm nop; __asm nop; __asm nop;
  }
}
