#include "Include.h"


uchar_8 SCI_flag=0;
uchar_8 Angle_flag=0;
uchar_8 SpeedControl_flag=0;
uchar_8 DirectionControl_flag=0;
uchar_8 AD_flag=0;
uchar_8 Speed_flag=0;
uchar_8 g_n1MsEventCount=0;
uchar_8 g_nSpeedControlPeriod=0;
uchar_8 g_nSpeedControlCount=0;
uchar_8 g_nDirectionControlCount=0;
uchar_8 g_nDirectionControlPeriod=0;

uint_16 Senddata[7]={0,0,0,0,0,0,0};
extern uint_16 Buffer[3];
extern float AD_Max[3][3];
extern uint_16 AngleResult[4];
extern uint_16 Adcalcalation[5];
extern uint_16 ADResult[4];
extern float AngleCalculateResult[4];
extern uchar_8 Speed[2];
extern char Turnstand[2];
extern int Leftspeed;
extern int Rightspeed;
extern float AD_Cal[3][3]; 
<<<<<<< HEAD
extern uint_16 stop_flag;
extern uint_16 dis;
extern float Speedset;  
//extern uint_16 Gravity_Offset;
//extern uint_16 Gyrscope_Offset;
//extern uint_16 Turn_Gravity_Offset;
//extern uint_16 Turn_Angle_Offset;
=======
extern uint_16 zhidao_flag; //直道元素标记
extern uint_16 Swan_flag;//S弯标记
extern uint_16 shizi_flag;//十字
extern uint_16 left_flag;//左拐
extern uint_16 right_flag;//右拐
extern uint_16 stop_flag;//停车标志
extern uint_16 start_flag;//出发标志
extern float distance;
extern float distance_nofilter;
>>>>>>> 60d881f1213707a13672e023a0d870fa7f13b89f

void main(void) 
{
  DisableInterrupts;
  //MCU初始化
  PLL_INIT();
  PIT_INIT();
  PWM_INIT();
  AD_INIT();
  IO_INIT();
  SCI_INIT();
  //LCD_Init();
  DFLASH_INIT();
  
  EnableInterrupts;
  
  for(;;) 
  {
     if(PORTB_PB0==0) 
     {
          AD_biaoding();
     } 
     else if(PORTB_PB2==0) 
     {
<<<<<<< HEAD
        stop_flag=0;
        Speedset=360;
       /* LCD_P6x8Str(0,0,"Angle:"); 
=======
        /*LCD_P6x8Str(0,0,"Angle:"); 
>>>>>>> 60d881f1213707a13672e023a0d870fa7f13b89f
        LCD_P6x8Str(0,0,"Gravity_Offset");
        LCD_P6x8Str(0,1,"Anglespeed:");
        LCD_P6x8Str(67,1,"Gyrscope_Offset"); 
        LCD_P6x8Str(0,2,"Anglespeed:");
        LCD_P6x8Str(66,2,"123"); 
        LCD_P6x8Str(0,3,"Kp:");
        LCD_P6x8Str(18,3,"123");
        LCD_P6x8Str(0,4,"Kd:");
<<<<<<< HEAD
        LCD_P6x8Str(18,4,"123");*/    
=======
        LCD_P6x8Str(18,4,"123");  */
        stop_flag=0;      
>>>>>>> 60d881f1213707a13672e023a0d870fa7f13b89f
     }
     else if(PORTB_PB3==0)
     {
        SCI_senddata();
        Senddata[0]=AngleResult[0];
        Senddata[1]=AngleResult[1];
<<<<<<< HEAD
        Senddata[2]=dis;//右轮
        Senddata[3]=Adcalcalation[0];//左轮 
        Senddata[4]=Adcalcalation[1];//AngleResult[3];//Turnstand[0];
        Senddata[5]=Adcalcalation[2];//Turnstand[1];
        Senddata[6]=AngleResult[3];
=======
        Senddata[2]=distance_nofilter;//右轮
        Senddata[3]=distance;//左轮 
        Senddata[4]=Adcalcalation[0];//AngleResult[3];//Turnstand[0];
        Senddata[5]=Adcalcalation[1];//Turnstand[1];
        Senddata[6]=Adcalcalation[2];
>>>>>>> 60d881f1213707a13672e023a0d870fa7f13b89f
      } 
      else if(PORTB_PB4==0) 
      {
             AD_Max[0][0]=Dflash_Read(data_Address); 
             AD_Max[0][1]=Dflash_Read(data_Address+2); 
             AD_Max[0][2]=Dflash_Read(data_Address+4); 
             AD_Max[1][0]=Dflash_Read(data_Address+6); 
             AD_Max[1][1]=Dflash_Read(data_Address+8);
             AD_Max[1][2]=Dflash_Read(data_Address+10);
             AD_Max[2][0]=Dflash_Read(data_Address+12);
             AD_Max[2][1]=Dflash_Read(data_Address+14);
             AD_Max[2][2]=Dflash_Read(data_Address+16); 
             AD_Cal[1][0]=AD_Max[1][1];
             AD_Cal[1][1]=(AD_Max[0][1]+AD_Max[2][1])/2;
      }
     
     /*if(AD_flag==1) 
     {
        Adget();
        AD_flag=0;
     } 
     else if(Speed_flag==1) 
     {
        SpeedCalculate();
        Speed_flag=0;
     } 
     else if(Angle_flag==1) 
     {   
        GetAngle(AngleResult);
        AngleControl();
        Angle_flag=0; 
     } 
     /*else if(SCI_flag==10) 
     {
         SCI_senddata();
         SCI_flag=0;
     } */            
     _FEED_COP(); 
  } 
}

#pragma CODE_SEG __NEAR_SEG NON_BANKED
void interrupt VectorNumber_Vpit0 pit0(void)    //1ms中断
{
     g_n1MsEventCount++;
     g_nSpeedControlPeriod++;     
     g_nDirectionControlPeriod++;
     SpeedControlOutput();
     MotorOutput();
     DirectionControlOutput();
     if(g_n1MsEventCount >= 5)
     {
         g_n1MsEventCount = 0;      
         SpeedCalculate();                    
     }
     else if(g_n1MsEventCount == 1) 
     {
          GetAngle(AngleResult);    
     }
     else if(g_n1MsEventCount == 2)
     {      
         AngleControl(); 
     }
     else if(g_n1MsEventCount == 3) 
     {
          g_nSpeedControlCount++;
          if(g_nSpeedControlCount >= 20) 
          {
              SpeedControl();
              g_nSpeedControlCount = 0;
              g_nSpeedControlPeriod = 0;     
           }
      }
      else if(g_n1MsEventCount == 4)
      {
         g_nDirectionControlCount++;
         if(g_nDirectionControlCount >= 2) 
         {
             DirectionControl();
             g_nDirectionControlCount = 0;
             g_nDirectionControlPeriod = 0;
          }
      }              
      PITTF_PTF0 = 1; 
}
#pragma CODE_SEG DEFAULT
