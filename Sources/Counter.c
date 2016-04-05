 #include "Include.h"
/*************��������*****************/
uchar_8 Speed[2];
char Turnstand[2];
int Lastleftspeed,Lastrightspeed;
int Nowleftspeed,Nowrightspeed;
int Leftspeed,Rightspeed; 
/*************��ȡ������*****************/
void GetMotorSpeed(unsigned char *speed) 
{ 
    int i;
    uchar_8 leftspeed,rightspeed;
    PORTB_PB6=0;//���������
    for(i=0;i<60;i++)//��ʱ 
    { _asm(nop); }
    leftspeed=PORTA;
    *speed=leftspeed;
    speed++;
    PORTB_PB6=1;
    
    PORTB_PB7=0;//���Ҽ�����
    for(i=0;i<60;i++)//��ʱ 
    { _asm(nop); }
    rightspeed=PORTA;
    *speed=rightspeed;
    PORTB_PB7=1;
}       
/*************��ȡD������*****************/ 
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