#include "Include.h"

/*************直立控制参数*****************/
extern float AngleCalculateResult[4];
float AngleKp=0;
float AngleKi=0;
float AngleKd=0;
float g_fGravityAngle=0;
float g_fGyroscopeAngleSpeed=0;
float GravityAngleInit=0;
float GyroscopeAngleSpeedInit=0;
float g_fAngleControlOut=0;
float LastAnglespeed=0;

/*************速度控制参数*****************/
extern int Leftspeed,Rightspeed;
extern uchar_8 g_nSpeedControlPeriod;
float Speedset=150;
float SpeedKp=0;//.094;//0.032;
float SpeedKd=0;
float SpeedKi=0;//0.000005;
float g_fCurrentSpeed;
float SpeedDelta;
float DeltaValue[2];
float g_fSpeedControlIntegral;
float g_fSpeedControlOutOld;
float g_fSpeedControlOutNew;
float g_fSpeedControlOut;

/*************方向控制参数*****************/
float g_fDirectionControlOutNew;
float g_fDirectionControlOutOld;
float g_fDirectionControlOut;
float g_fDirectionError;
float g_fDirectionErrorOld;
float Delta_DirectionError;
float DirectionAngleSpeedK;
float DirectionKp=0.0019;//0.0045;//0.0057;//=9;
float DirectionKi=0;
float DirectionKd=0.00012;//0.0006;//0.0003;//0.00027;
float I_DirectionError;
extern uchar_8 g_nDirectionControlPeriod;
/*************直立控制输出函数*****************/
void AngleControl(void) 
{
  float Delta_Angle;
  float Delta_AngleSpeed;
  float Value_Angle,Value_AngleSpeed;
  float NowAngle;
  float NowAngleSpeed;
  AngleCalculate();
  
  g_fGravityAngle=AngleCalculateResult[0];     //读取角度
  g_fGyroscopeAngleSpeed=AngleCalculateResult[1];//读取角速度
   
  Value_Angle=GravityAngleInit-g_fGravityAngle;
  Value_AngleSpeed=GyroscopeAngleSpeedInit-g_fGyroscopeAngleSpeed;
 /* if(Value_Angle<-5) 
  {
    AngleKp=0.07646;
    AngleKd=0.0000126;
  }
  else if(Value_Angle>0.7) 
  {
    AngleKp=0.23572;
    AngleKd=0.000073;
  }
  else                
  {    */
  if(Value_Angle<=0)
   AngleKp=0.7;//0.9855;//0.9855;//0.99
  else//0.00450
   AngleKp=0.65;//0.9764;//0.9764;
  AngleKd=0.00002;     //0.000003220
  NowAngle=g_fGravityAngle;
  NowAngleSpeed=g_fGyroscopeAngleSpeed;
  Delta_AngleSpeed=NowAngleSpeed-LastAnglespeed;
  LastAnglespeed=NowAngleSpeed;
  Delta_Angle=AngleKp*(GravityAngleInit-g_fGravityAngle)+AngleKd*0.8*(GyroscopeAngleSpeedInit-g_fGyroscopeAngleSpeed)+AngleKd*0.2*Delta_AngleSpeed;
  //Delta_Angle=AngleKp*(GravityAngleInit-g_fGravityAngle)+AngleKd*(GyroscopeAngleSpeedInit-g_fGyroscopeAngleSpeed);
  g_fAngleControlOut=Delta_Angle;
}

/*************速度控制输出函数*****************/
void SpeedSet() 
{
   if(Speedset==0)
   {  
     SpeedKp=0.0113;
     SpeedKd=0.00004;
     SpeedKi=0.00005;   
   }
   else
   {
     SpeedKp=0.0165;
     SpeedKd=0;
     SpeedKi=0.00003;   
   }
}
void SpeedControl(void)
{
    float Pvalue,Ivalue,Dvalue;
    g_fCurrentSpeed=(Leftspeed+Rightspeed)/2;//均值
    SpeedSet();
    Leftspeed=Rightspeed=0;//清零
    SpeedDelta=Speedset-g_fCurrentSpeed;
    
    DeltaValue[0]=SpeedDelta;
    
    
    Pvalue=SpeedDelta*SpeedKp;
    Dvalue=(DeltaValue[0]-DeltaValue[1])*SpeedKd;
    Ivalue=SpeedDelta*SpeedKi;
    
    DeltaValue[1]=DeltaValue[0];
    g_fSpeedControlIntegral+=Ivalue;
    if(g_fSpeedControlIntegral>0.010) g_fSpeedControlIntegral=0.010;
    if(g_fSpeedControlIntegral<-0.010) g_fSpeedControlIntegral=-0.010;  //速度积分很容易达到饱和
    
    g_fSpeedControlOutOld=g_fSpeedControlOutNew;
    g_fSpeedControlOutNew=Pvalue+Dvalue+g_fSpeedControlIntegral;
}
void SpeedControlOutput(void) 
{
    float fValue;
    fValue=g_fSpeedControlOutNew-g_fSpeedControlOutOld;
    g_fSpeedControlOut=fValue*(g_nSpeedControlPeriod+1)/100+g_fSpeedControlOutOld;
 }
/*************方向控制输出函数*****************/
void DirectionControl(void) 
{																		
    float DirectionValue; 
    Position();//Position();
                  
    g_fDirectionControlOutOld=g_fDirectionControlOutNew;
    Delta_DirectionError=g_fDirectionError-g_fDirectionErrorOld;
    DirectionValue=AngleCalculateResult[3];
    
    g_fDirectionErrorOld=g_fDirectionError; 
    I_DirectionError+=DirectionKi*Delta_DirectionError;
    if(I_DirectionError>=0.01)
      I_DirectionError=0.01;
    else if(I_DirectionError<=-0.01)
      I_DirectionError=-0.01;
    
    g_fDirectionControlOutNew= g_fDirectionError*DirectionKp+(g_fDirectionError-g_fDirectionErrorOld)*DirectionKd+I_DirectionError-DirectionAngleSpeedK*DirectionValue/100;

}
void DirectionControlOutput(void) 
{
    float Value;
    Value=g_fDirectionControlOutNew-g_fDirectionControlOutOld;
    g_fDirectionControlOut=Value*g_nDirectionControlPeriod/10+g_fDirectionControlOutOld;
}

