 #include "Include.h"
/*************变量定义*****************/
uchar_8 Speed[2];
char Turnstand[2];
int Lastleftspeed,Lastrightspeed;
int Nowleftspeed,Nowrightspeed;
int Leftspeed,Rightspeed; 
/*************读取计数器*****************/
void GetMotorSpeed(unsigned char *speed) 
{ 
    int i;
    uchar_8 leftspeed,rightspeed;
    PORTB_PB6=0;//读左计数器
    for(i=0;i<60;i++)//延时 
    { _asm(nop); }
    leftspeed=PORTA;
    *speed=leftspeed;
    speed++;
    PORTB_PB6=1;
    
    PORTB_PB7=0;//读右计数器
    for(i=0;i<60;i++)//延时 
    { _asm(nop); }
    rightspeed=PORTA;
    *speed=rightspeed;
    PORTB_PB7=1;
}       
/*************读取D触发器*****************/ 
void GetSpeedVeer(char *speedVeer) 
{
   char rspeedveer,lspeedveer;
   lspeedveer=PTIJ_PTIJ6;
   *speedVeer=lspeedveer;
   speedVeer++;
   rspeedveer=PTIJ_PTIJ7;
   *speedVeer=rspeedveer;
}
void SpeedCalculate(void) 
{
  int Lspeed,Rspeed;
  
  GetMotorSpeed(Speed);
  GetSpeedVeer(Turnstand);
  Nowrightspeed=Speed[0];
  Rspeed=Nowrightspeed;
  if(Nowrightspeed<Lastrightspeed) 
    Nowrightspeed=Nowrightspeed+255-Lastrightspeed;
  else
    Nowrightspeed=Nowrightspeed-Lastrightspeed;
  Lastrightspeed=Rspeed;
  if(Turnstand[0]==1)
  Nowrightspeed=-Nowrightspeed;
  Rightspeed+=Nowrightspeed;
  
  Nowleftspeed=Speed[1];
  Lspeed=Nowleftspeed;
  if(Nowleftspeed<Lastleftspeed) 
    Nowleftspeed=Nowleftspeed+255-Lastleftspeed;
  else
    Nowleftspeed=Nowleftspeed-Lastleftspeed;
  Lastleftspeed=Lspeed;
  if(Turnstand[1]==0)
  Nowleftspeed=-Nowleftspeed;
  Leftspeed+=Nowleftspeed;
  
}