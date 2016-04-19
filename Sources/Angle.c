#include "Include.h"

extern uint_16 Adcalcalation[5];

//uint_16 Gravity_Offset=1796;
//uint_16 Gyrscope_Offset=1408;
//uint_16 Turn_Gravity_Offset=1224;
//uint_16 Turn_Angle_Offset=0;
uint_16 AngleResult[4];
uint_16 ADResult[4];
float AngleCalculateResult[4];
/*************��ȡ���ٶȺͽǼ��ٶ�*****************/
void GetAngle(uint_16 *Angle) 
{
    int i;
    uint_16 value[9]={0};//����
    for(i=0;i<20;i++) 
    {
      while(!ATD0STAT0_SCF);      
      value[0]+=ATD0DR8;//������ֱ���Ƕȣ����
      value[1]+=ATD0DR9;//Z����ٶ�
      value[2]+=ATD0DR6;//ת��Ƕ�
      value[3]+=ATD0DR7;//ת����ٶ�
      value[4]+=ATD0DR0;//����
      value[5]+=ATD0DR1;//�м�
      value[6]+=ATD0DR2;//�ҵ��
      value[7]+=ATD0DR3; //�ҵ��1
      value[8]+=ATD0DR4;//�ҵ��2
    }
    value[0]=value[0]/20;
    value[1]=value[1]/20;
    value[2]=value[2]/20;
    value[3]=value[3]/20;
    value[4]=value[4]/20;
    value[5]=value[5]/20;
    value[6]=value[6]/20;
    value[7]=value[7]/20;
    value[8]=value[8]/20;
    ADResult[0]=5000/1024*value[0];//���й�һ����AD��������Ϊ10λ
    ADResult[1]=5000/1024*value[1];
    ADResult[2]=5000/1024*value[2];
    ADResult[3]=5000/1024*value[3];
  
    Adcalcalation[0]=value[4];
    Adcalcalation[1]=value[5];
    Adcalcalation[2]=value[6];
    
    for(i=0;i<4;i++)
    *(Angle++)=ADResult[i];
}

void AngleCalculate(void)
{
  AngleCalculateResult[0]=((float)AngleResult[0]-Gravity_Offset)/Gravity_Angle_Ratio;//�����ǽǶ� ���
  AngleCalculateResult[1]=((float)AngleResult[1]-Gyrscope_Offset)/Gyrscope_Angle_Ratio;//Z����ٶ�
  AngleCalculateResult[2]=((float)AngleResult[2]-Turn_Angle_Offset)/Turn_Angle_Ratio;//ת��Ƕ�
  AngleCalculateResult[3]=((float)AngleResult[3]-Turn_Gravity_Offset)/0.67;//ת��Ǽ��ٶ�
}
  
/*void Angleoffset() 
{
     if(PORTE_PE0==0) 
     {
         delay(10);
         if(PORTE_PE0==0) 
         {
            
         }
     }

}                 */