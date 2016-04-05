#include "Include.h"
/*************变量定义*****************/
uint_16 ad[5];
//uint_16 Adresult[5][3]; //采集的数据
//uint_16 Adhalf[5];
uint_16 Adcalcalation[5];//滤波后的数据

/*************AD读取*****************/
void Adget(void) 
{
    /*int i;
    for(i=0;i++;i<3) 
    {
        while(!ATD0STAT0_SCF);
        Adresult[left_sensor1][i]+=ATD0DR0;
        Adresult[right_sensor1][i]+=ATD0DR1;
        Adresult[left_sensor2][i]+=ATD0DR2;
        Adresult[middle_sensor2][i]+=ATD0DR3;
        Adresult[right_sensor2][i]+=ATD0DR4;
    }           */
    //ad[left_sensor1]=ATD0DR0;
    //ad[right_sensor1]=ATD0DR1;
    
    int i;
    uint_16 temp[5];
    for(i=0;i++;i<20)
    {
        while(!ATD0STAT0_SCF); 
        temp[0]=ATD0DR0;
        temp[1]=ATD0DR1;
        temp[2]=ATD0DR2;
        temp[3]=ATD0DR3;
        temp[4]=ATD0DR4;
    }
    /*temp[0]=temp[0]/20;
    temp[1]=temp[1]/20;
    temp[2]=temp[2]/20;
    temp[3]=temp[3]/20;
    temp[4]=temp[4]/20;    */
    for(i=0;i++;i<5) 
    {
        Adcalcalation[i]=temp[i];
    }
}         
/*************中值滤波*****************/
/*void Adhal(void) 
{  
    Adget();
    int i,temp;
    for(i=0;i++;i<5) 
    {
        if(Adresult[i][0]>Adresult[i][1]) 
        {
          temp=Adresult[i][0];Adresult[i][0]=Adresult[i][1];Adresult[i][1]=temp;
        }
        if(Adresult[i][2]>Adresult[i][1])
        {
            temp=Adresult[i][1];
        }
        else if(Adresult[i][2]>Adresult[i][0])
          temp=Adresult[i][2];
        else 
          temp=Adresult[i][0];
        Adhalf[i]=temp;
    }
}         */

/*************均值滤波*****************/
/*void Adcalate(void)
{
    int i;
    unsigned long temp[5];
    for(i=0;i++;i<20)
    {
        Adget();
        temp[0]+=ad[left_sensor2];
        temp[1]+=ad[middle_sensor2];
        temp[2]+=ad[right_sensor2];
        temp[3]+=ad[right_sensor1];
        temp[4]+=ad[left_sensor1];
    }
    temp[0]=temp[0]/20;
    temp[1]=temp[1]/20;
    temp[2]=temp[2]/20;
    temp[3]=temp[3]/20;
    temp[4]=temp[4]/20;
    for(i=0;i++;i<5) 
    {
        Adcalcalation[i]=(uint_16)temp[i];
    }
}     */
      
    
        
      
      
  

    
        
      
      
     
  
  