#include "Include.h"

extern uint_16 Adcalcalation[5];
extern uint_16 Buffer[3];
extern float g_fDirectionError;
extern float DirectionKp;//0.007;//=9;
extern float DirectionKi;
extern float DirectionKd;//0.0004;
float AD_Max[3][3];//�궨ֵ
float AD_Cal[3][2];//λ�Ƽ���
float distance;//λ��ֵ
float distance_nofilter;//û�˲���λ��ֵ
float AD_Min=10;//��С���ֵ��С�����ֵ˵����в��ɿ�
float distance_abs[3];//�����߾���ľ���ֵ
float distance_max=500;
uint_16 zhidao_flag; //ֱ��Ԫ�ر��
uint_16 Swan_flag;//S����
uint_16 shizi_flag;//ʮ��
uint_16 left_flag;//���
uint_16 right_flag;//�ҹ�
uint_16 stop_flag;//ͣ����־
uint_16 start_flag;//������־


/*
AD_Max[i][j] Ϊ�궨����
i=0��ʾ���������ұߣ��������;
i=1��ʾ���������м䣬�м������
i=2��ʾ����������ߣ��ҵ�����

j=0��ʾ����
j=1��ʾ�м���
j=2��ʾ�ҵ��


*/
/*************��б궨����*****************/
void AD_biaoding() 
{
   int i,j;
   float temp[3]; //�ݴ����ֵ
   for(i=0;i<3;i++) 
   {
        for(j=0;j<3;j++) 
        {
            AD_Max[i][j]=0;//������
        }
        temp[i]=0;
   }
   if(PORTB_PB0==0) 
   {
        while(PORTB_PB0==0) 
        {
            temp[0]=temp[0]*0.5+0.5*Adcalcalation[0]; //ˢ�µ��ֵ������Ȩֵ
            temp[1]=temp[1]*0.5+0.5*Adcalcalation[1];
            temp[2]=temp[2]*0.5+0.5*Adcalcalation[2];
            for(i=0;i<3;i++) 
            {
                if(temp[i]>AD_Max[i][i]) 
                {
                    for(j=0;j<3;j++) 
                    {
                        AD_Max[i][j]=temp[j];     //�궨
                    }
                }
            }
        }
        Dflash_Erase(data_Address);     //ȷ���Ȳ�����д��
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

/*************������㺯��*****************/
void Delt_Distance() 
{
    static int sign=1;//�ж�λ����������������������
    static float distance_pre;//��һ��λ��
    static int sign_change;
    float max_num,min_num,temp_max,temp_min;//���ڼ����ֵ���м����
    float temp,temp1,temp2,temp3,temp4;//���ڼ�����м����
    static float right=0,left=0;//��ͨ�˲����ж�����
    
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
    if(distance_abs[0]<0) distance_abs[0]=0; //ȷ�ϸ�ֵΪ��
    if(distance_abs[1]<0) distance_abs[1]=0;
    if(distance_abs[2]<0) distance_abs[2]=0;
    
    right=0.9*right+0.1*Adcalcalation[right_sensor2]; //��ͨ�˲�
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
                          if(right>left)  sign=1;    //�������ұ�
                          else if(right<left) sign=-1;//���������
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
    if(distance_abs[1]<42)      //���´������ں���������ľ���ֵ
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


    //distance-=20;    //��λ��������֧�ܰ�װ�������
    if(distance > distance_max) distance = distance_max;   //�޶�λ�Ƶļ�ֵ
    else if(distance < -distance_max) distance = -distance_max;
    distance_nofilter = distance;    //�۲�δ�˲���ֵ��������
    distance = 0.1 * distance + 0.9 * distance_pre; //�Լ�������ͨ�˲���ʹ���ƽ��
    distance_pre = distance;  
    g_fDirectionError=distance;

}
void Setparamater() 
{
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
        
void Position() 
{
   uint_16 stop_time;
   uint_16 turnleft_time;
   uint_16 turnright_time;
   uint_16 swan_time;
   if(Adcalcalation[right_sensor2]<20&&Adcalcalation[left_sensor2]<20&&Adcalcalation[middle_sensor2]<20) 
   {
      stop_time++;
      if(stop_time>10) 
      {
          stop_flag=1;
          stop_time=0;
      }
   } 
   else if(Adcalcalation[right_sensor2]>100&&Adcalcalation[middle_sensor2]>100&&Adcalcalation[left_sensor2]<70) 
   {
      turnright_time++;
      if(turnright_time>10) 
      {
          right_flag=1;
          turnright_time=0;
      }
   }
   else if(Adcalcalation[left_sensor2]>100&&Adcalcalation[middle_sensor2]>100&&Adcalcalation[right_sensor2]<70)
   {
      turnleft_time++;
      if(turnleft_time>10) 
      {
          left_flag=1;
          turnleft_time=0;
      }
   }
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