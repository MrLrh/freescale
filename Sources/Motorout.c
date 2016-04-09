#include "Include.h"

extern float g_fAngleControlOut;
extern float g_fSpeedControlOut;
extern float g_fDirectionControlOut;
float g_fLeftMotorOut,g_fRightMotorOut;
float MOTOR_OUT_MAX=12;
float MOTOR_OUT_MIN=-12; //限幅
int LeftFrontDead_Val=0;//487;//487;                  
int LeftBackDead_Val=0;//445;//467;
int RightFrontDead_Val=0;//408;//478;
int RightBackDead_Val=0;//415;//480;
int nOutL,nOutR; 

void MotorOutput(void);  
void MotorSpeedControl(void);   
void SetMotorPWM(int leftPWM,int rightPWM);
 
/**********************电机输出**************************************/
void MotorOutput(void)
{
   float  fLeft, fRight;
   fLeft =g_fAngleControlOut-g_fSpeedControlOut-g_fDirectionControlOut; //右电机
   fRight=g_fAngleControlOut-g_fSpeedControlOut+g_fDirectionControlOut; //左电机
   if(PORTB_PB1==1) 
   {
    fLeft=0;
    fRight=0;
   }
   

   g_fLeftMotorOut=fLeft;                                                         
   g_fRightMotorOut=fRight;
   MotorSpeedControl();
   
}

void MotorSpeedControl(void) 
{
  float fLeftVal, fRightVal;
  int  nPeriod=250;
 
  fLeftVal=g_fLeftMotorOut;
  fRightVal=g_fRightMotorOut;
 
	if(fLeftVal > MOTOR_OUT_MAX)
		fLeftVal = MOTOR_OUT_MAX;
	if(fLeftVal < MOTOR_OUT_MIN)
		fLeftVal = MOTOR_OUT_MIN;
	if(fRightVal > MOTOR_OUT_MAX)
		fRightVal = MOTOR_OUT_MAX;
	if(fRightVal < MOTOR_OUT_MIN)
		fRightVal = MOTOR_OUT_MIN;
	
	nOutL=(int)(fLeftVal*nPeriod);
  nOutR=(int)(fRightVal*nPeriod);
  SetMotorPWM(nOutL,nOutR);
}

void SetMotorPWM(int leftPWM,int rightPWM) 
{
  if(leftPWM<0) 		  
  {
    PWMDTY01=0;
    PWMDTY23=-nOutL+LeftFrontDead_Val;
  }
  else 
  {
    PWMDTY01=nOutL+LeftBackDead_Val;
    PWMDTY23=0; 
    
  }
  if(rightPWM<0) 
  {
    PWMDTY45=0; 
    PWMDTY67=-nOutR+RightFrontDead_Val; 
  }
  else 
  {
    PWMDTY45=nOutR+RightBackDead_Val;
    PWMDTY67=0;
    
  }
}  
  