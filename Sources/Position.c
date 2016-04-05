#include "Include.h"

extern uint_16 Adcalcalation[5];
extern uint_16 Buffer[3];
extern float g_fDirectionError;
extern float DirectionKp;//0.007;//=9;
extern float DirectionKi;
extern float DirectionKd;//0.0004;
float AD_Max[3][3];//标定值
float AD_Cal[3][2];//位移计算
float distance;//位移值
float distance_nofilter;//没滤波的位移值
float AD_Min;//最小电感值，小于这个值说明电感不可靠
float distance_abs[3];//到导线距离的绝对值
float distance_max=500;


/*
AD_Max[i][j] 为标定数组
i=0表示车子在最右边，左电感最大;
i=1表示车子在最中间，中间电感最大
i=2表示车子在最左边，右电感最大

j=0表示左电感
j=1表示中间电感
j=2表示右电感


*/
/*************电感标定函数*****************/
void AD_biaoding() 
{
   int i,j;
   float temp[3]; //暂存最大值
   for(i=0;i<3;i++) 
   {
        for(j=0;j<3;j++) 
        {
            AD_Max[i][j]=0;//先清零
        }
        temp[i]=0;
   }
   if(PORTB_PB0==0) 
   {
        while(PORTB_PB0==0) 
        {
            temp[0]=temp[0]*0.5+0.5*Adcalcalation[0]; //刷新电感值，加上权值
            temp[1]=temp[1]*0.5+0.5*Adcalcalation[1];
            temp[2]=temp[2]*0.5+0.5*Adcalcalation[2];
            for(i=0;i<3;i++) 
            {
                if(temp[i]>AD_Max[i][i]) 
                {
                    for(j=0;j<3;j++) 
                    {
                        AD_Max[i][j]=temp[j];     //标定
                    }
                }
            }
        }
        Dflash_Erase(data_Address);     //确保先擦除后写入
        Buffer[0]=AD_Max[0][0];
        Buffer[1]=AD_Max[0][1];
        Buffer[2]=AD_Max[0][2];
        Dflash_Write(data_Address,0);
        Buffer[0]=AD_Max[1][0];
        Buffer[1]=AD_Max[1][1];
        Buffer[2]=AD_Max[1][2];
        Dflash_Write(data_Address,1);
        Buffer[0]=AD_Max[2][0];
        Buffer[1]=AD_Max[2][1];
        Buffer[2]=AD_Max[2][2];
        Dflash_Write(data_Address,2);
        AD_Cal[1][0]=AD_Max[1][1];
        AD_Cal[1][1]=(AD_Max[0][1]+AD_Max[2][1])/2;
   } 
}

/*************距离计算函数*****************/
void Delt_Distance() 
{
    static int sign=1;//判断位移正负，即导线在左还是右
    static float distance_pre;//上一次位移
    static int sign_change;
    float max_num,min_num,temp_max,temp_min;//用于计算插值的中间变量
    float temp,temp1,temp2,temp3,temp4;//用于计算的中间变量
    static float right=0,left=0;//低通滤波后判断左右
    
    if(Adcalcalation[right_sensor2]>AD_Max[2][2])
         distance_abs[0]=0;
    else if(Adcalcalation[right_sensor2]>AD_Max[1][2]) 
    {
         max_num=AD_Max[1][2];
         min_num=AD_Max[2][2];
         temp_max=100;
         temp_min=0;
         temp=Adcalcalation[right_sensor2];
         distance_abs[0]=(temp-min_num)*(temp_max-temp_min)/(max_num-min_num)+temp_min;
    } 
    else 
    {
         max_num=AD_Max[0][2];
         min_num=AD_Max[1][2];
         temp_max=200;
         temp_min=100;
         temp=Adcalcalation[right_sensor2];
         distance_abs[0]=(temp-min_num)*(temp_max-temp_min)/(max_num-min_num)+temp_min;
    }
    distance_abs[1]=100*(Adcalcalation[middle_sensor2]-AD_Cal[1][0])/(AD_Cal[1][1]-AD_Cal[1][0]);
    if(Adcalcalation[left_sensor2]>AD_Max[0][0])
         distance_abs[2]=0;
    else if(Adcalcalation[left_sensor2]>AD_Max[1][0]) 
    {
         max_num=AD_Max[1][0];
         min_num=AD_Max[0][0];
         temp_max=100;
         temp_min=0;
         temp=Adcalcalation[left_sensor2];
         distance_abs[2]=(temp-min_num)*(temp_max-temp_min)/(max_num-min_num)+temp_min;
    } 
    else 
    {
         max_num=AD_Max[2][0];
         min_num=AD_Max[1][0];
         temp_max=200;
         temp_min=100;
         temp=Adcalcalation[left_sensor2];
         distance_abs[2]=(temp-min_num)*(temp_max-temp_min)/(max_num-min_num)+temp_min; 
    }
    if(distance_abs[0]<0) distance_abs[0]=0; //确认该值为正
    if(distance_abs[1]<0) distance_abs[1]=0;
    if(distance_abs[2]<0) distance_abs[2]=0;
    
    right=0.9*right+0.1*Adcalcalation[right_sensor2]; //低通滤波
    left=0.9*left+0.1*Adcalcalation[left_sensor2];
    
    if(right>AD_Min*5||left>AD_Min*5||distance>200||distance<-200) 
    {
         sign_change++;
         if(sign_change>10) 
         {
              sign_change=10;
              if(sign==1) 
              {
                    if(right>AD_Min||Adcalcalation[right_sensor2]>AD_Min) 
                    {
                          if(right>left)  sign=1;    //导线在右边
                          else if(right<left) sign=-1;//导线在左边
                    }
                    
              } 
              else 
              {
                    if(left>AD_Min||Adcalcalation[left_sensor2]>AD_Min) 
                    {
                           if(right>left) sign=1; 
                           else if(right<left) sign=-1;
                    }
              }
               
         }
    } 
    else
        sign_change=0;
    if(distance_abs[1]<42)      //以下代码是融合三个距离的绝对值
    {
        if(distance_abs[0]>distance_abs[2])
            temp1=(distance_abs[0]-distance_abs[2] ) / 2;
        else  temp1=-(distance_abs[0] -distance_abs[2] ) / 2;
        temp2=distance_abs[1];
        temp3=1;
        temp4=1 - temp3;
    }
    else if(distance_abs[1] < 58)
    {
        if(distance_abs[0]>distance_abs[2])
            temp1=(distance_abs[0] -distance_abs[2] ) / 2;
        else  temp1 =-(distance_abs[0]-distance_abs[2] ) / 2;
        temp2=distance_abs[1];
        temp3=(float)(58-distance_abs[1]) / 20;
        temp4=1-temp3;
    }
    else if(distance_abs[1]<92)
    {
        temp1=distance_abs[1];
        temp2=100+distance_abs[2];
        temp3=1;
        temp4=1-temp3;
        
    }
    else if(distance_abs[1]<108)
    {
        temp1=distance_abs[1];
        if(sign==-1)
            temp2=100+distance_abs[2];
        else temp2=100+distance_abs[0];
        temp3=(float)(108-distance_abs[1])/20;
        temp4=1-temp3;
    }
    else
    {
        temp1=distance_abs[1];
        if(sign==-1)
            temp2=100+distance_abs[2];
        else temp2=100+distance_abs[0];
        temp3=0;
        temp4=1-temp3;
    }
    temp=temp1*temp3+temp2*temp4;
    distance=temp*sign;
    
    if(distance<83&&distance>-83)       
    {
        temp=((float)(Adcalcalation[right_sensor2]-Adcalcalation[left_sensor2])*100)/(Adcalcalation[right_sensor2]+Adcalcalation[left_sensor2]);
        if(temp<50&&temp>-50)
            distance=temp;
        else if(temp<75&&temp>-75)
        {
            temp1 = distance;
            temp2 = temp;
            if(temp > 0)
                temp3 = ((float)(temp - 50)) / (75 - 50);
            else temp3 = ((float)(temp + 50)) / (50 - 75);
            temp4 = 1 - temp3;
            distance= temp1 * temp3 + temp2 * temp4;

        }
    }/**/


    //distance-=20;    //零位修正，由支架安装误差引起
    if(distance > distance_max) distance = distance_max;   //限定位移的极值
    else if(distance < -distance_max) distance = -distance_max;
    distance_nofilter = distance;    //观测未滤波的值，调试用
    distance = 0.1 * distance + 0.9 * distance_pre; //对计算结果低通滤波以使其更平滑
    distance_pre = distance;  
    g_fDirectionError=distance;

}

void Position() 
{
   Delt_Distance();
   /*if(distance>-10&&distance<10) 
   {
       DirectionKp=0.007;//=9
       DirectionKd=0.0004;
   } 
   else if(distance>-20&&distance<20) 
   {
       DirectionKp=0.010;//=9
       DirectionKd=0.0005;
   } 
   else 
   {
       DirectionKp=0.016;//=9
       DirectionKd=0.0006;
   }      */
    
} 