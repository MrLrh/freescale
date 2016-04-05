#include "Include.h"

extern uint_16 Senddata[8];
uchar_8 Outdata[14];
/*************************************************************/
/*                       串口发送函数                        */
/*************************************************************/
void SCI_send(unsigned char data) 
{
  while(!SCI0SR1_TDRE);       //等待发送数据寄存器（缓冲器）为空
  SCI0DRL = data;             //把数据放入SCI数据存储器
}

/*************************************************************/
/*                  下位机                      */
/*************************************************************/
void Datatransition() 
{
   Outdata[0]=(uchar_8)(Senddata[0]);
   Outdata[1]=(uchar_8)(Senddata[1]);
   Outdata[2]=(uchar_8)(Senddata[2]);
   Outdata[3]=(uchar_8)(Senddata[3]);
   Outdata[4]=(uchar_8)(Senddata[4]);
   Outdata[5]=(uchar_8)(Senddata[5]);
   Outdata[6]=(uchar_8)(Senddata[6]);
   Outdata[7]=0x00;
   Outdata[8]=(uchar_8)(Senddata[0]>>8);
   Outdata[9]=(uchar_8)(Senddata[1]>>8);
   Outdata[10]=(uchar_8)(Senddata[2]>>8);
   Outdata[11]=(uchar_8)(Senddata[3]>>8);
   Outdata[12]=(uchar_8)(Senddata[4]>>8);
   Outdata[13]=(uchar_8)(Senddata[5]>>8);
   Outdata[14]=(uchar_8)(Senddata[6]>>8);   
   
}
void SendHex(uchar_8 hex) 
{
    unsigned char temp;
    temp = hex & 0x0F;
    if(temp < 10) 
    {
      SCI_send(temp + '0');
     }   
    else 
    {
      SCI_send(temp - 10 + 'A');
     }
     temp = hex >> 4;
     if(temp < 10) 
     {
       SCI_send(temp + '0');
      } 
     else 
     {
       SCI_send(temp - 10 + 'A');
      }
}
void SCI_senddata() 
{
   int i;
   Datatransition();
   SCI_send('=');
   SCI_send('=');
   for(i=0;i<15;i++)
      SendHex(Outdata[i]);
}


