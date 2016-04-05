#include "Include.h" 

#define BUS_CLOCK		   60000000	    //����Ƶ��
#define OSC_CLOCK		   16000000	    //����Ƶ��
#define BAUD 9600                   //���ڲ�����

/*************���໷��ʼ��*****************/
void PLL_INIT(void) 
{
   
    //clksel.7(PLLSELѡ��λ)�����ڲ�����ʱ����Դ  =0,BusClock=OSCCLK/2;   =1,BusClock=PLLCLK/2
   CLKSEL_PLLSEL=0;	  
           
   PLLCTL_PLLON=0;  //PLLCTL.6(pllon)��Ϊ0;�ȹر�PLL

    //������Ҫ��ʱ��Ƶ������SYNR��REFDV�Ĵ���  BusClock=PLLCLK/2;
    //���㹫ʽ: FVOC=2*OSCCLK*((SYNR+1)/(REFDV+1)) 
    //          PLLCLK=FVCO/(2*POSTDIV) 
    //          BusClock=PLLCLK/2
    //SYNR(.7.6)  32MHZ<FVOC<48MHZ   00   0
    //SYNR(.7.6)  48MHZ<FVOC<80MHZ   01   4
    //SYNR(.7.6)  80MHZ<FVOC<120MHZ  10   8
    //SYNR(.7.6)  120MHZ<FVOC        11   C		
   SYNR=0xC0|0x0E; 	
    //���㹫ʽ FREF=OSCCLK/(REDIV*2)	
    //REFDV(.7.6)  1MHZ<FREF<2MHZ    00  0
    //REFDV(.7.6)  2MHZ<FREF<6MHZ    01  4
    //REFDV(.7.6)  6MHZ<FREF<12MHZ   10  8
    //REFDV(.7.6)  12MHZ<FREF	       11  C
   REFDV=0x40|0x03;
 
   
   
  //�����Ĵ���PLLCLK=FVCO/(2*POSTDIV)   ���POSTDIV=0x00��PLLCLK=FVCO             
   POSTDIV=0x00;     

   PLLCTL_PLLON=1;  //PLLCTL.6(pllon)��Ϊ1;��PLL

   _asm(nop);         
   _asm(nop);
   while(CRGFLG_LOCK==0); //�Ÿ���CRGFLG�Ĵ�����LOCKλ��ȷ��PLL�Ƿ��ȶ�	 LOCK==1 �ȶ���==0 ���ȶ�  
   CLKSEL_PLLSEL =1;     //ѡ��PLL��Ϊʱ��Դ		       

}
/*************��ʱ���жϳ�ʼ��*****************/    
void PIT_INIT(void) 
{
  PITCFLMT_PITE=0;//��ʱͨ���ر�
  PITCE_PCE0=1;//��ʱ��ͨ��0ʹ��
  PITMTLD0=120-1;//8λ��ʱ����ֵ
  PITLD0=500-1;//16λ��ʱ����ֵ       1ms AD��ʱ�ж�
  PITINTE_PINTE0=1;//��ʱ���ж�ͨ��0�ж�ʹ��
 
 /* PITCE_PCE1=1;//��ʱ��ͨ��1ʹ��
  PITLD1=5000-1;//16λ��ʱ����ֵ        10ms  ��ȡ�ٶ��ж�
  PITINTE_PINTE1=1;//��ʱ���ж�ͨ��1�ж�ʹ��
  
  PITCE_PCE2=1;//��ʱ��ͨ��2ʹ��
  PITLD2=2500-1;//16λ��ʱ����ֵ        5ms �����ж�
  PITINTE_PINTE2=1;//��ʱ���ж�ͨ��2�ж�ʹ��   */
  PITCFLMT_PITE=1;//��ʱ��ͨ��0ʹ�� 
}
/*************PWM��ʼ��*****************/
void PWM_INIT(void)
{
  PWME=0x00;//��ֹPWM
  PWMPRCLK=0x00;//ʱ��Ԥ��Ƶ�Ĵ������� A=60M,B=60M
  PWMSCLA=0x02;//SA=A/2/2=15M    ʱ������
  PWMSCLB=0x02;//SB=B/2/2=15M    ʱ������
  PWMCTL_CON01=1;//0,1ͨ������
  PWMCTL_CON23=1;//2,3ͨ������
  PWMCTL_CON45=1;//4,5ͨ������
  PWMCTL_CON67=1;//6,7ͨ������
  PWMCLK_PCLK1=1;// PWM01-----SA             ʱ��Դ��ѡ��
  PWMPOL_PPOL1=1;//��������������Ǹߵ�ƽȻ���ٱ�Ϊ�͵�ƽ
  PWMCLK_PCLK3=1;// PWM23-----SB             ʱ��Դ��ѡ��
  PWMPOL_PPOL3=1;//��������������Ǹߵ�ƽȻ���ٱ�Ϊ�͵�ƽ
  PWMCLK_PCLK5=1;// PWM45-----SA             ʱ��Դ��ѡ��
  PWMPOL_PPOL5=1;//��������������Ǹߵ�ƽȻ���ٱ�Ϊ�͵�ƽ
  PWMCLK_PCLK7=1;// PWM67-----SB             ʱ��Դ��ѡ��
  PWMPOL_PPOL7=1;//��������������Ǹߵ�ƽȻ���ٱ�Ϊ�͵�ƽ
  PWMCAE_CAE1=0;//����� ���뷽ʽ����
  PWMCAE_CAE3=0;//����� ���뷽ʽ����
  PWMCAE_CAE5=0;//����� ���뷽ʽ����
  PWMCAE_CAE7=0;//����� ���뷽ʽ����
  PWMPER01=5000;//Ƶ��=SA/5000 3K    ������ת
  PWMDTY01=0;//ռ�ձ�Ϊ0
  PWMPER23=5000;//Ƶ��=SB/5000 3K    ������ת
  PWMDTY23=0;//ռ�ձ�Ϊ0
  PWMPER45=5000;//Ƶ��=SA/5000 3K    �ҵ����ת
  PWMDTY45=0;//ռ�ձ�Ϊ0
  PWMPER67=5000;//Ƶ��=SB/5000 3K    �ҵ����ת      
  PWMDTY67=0;//ռ�ձ�Ϊ0
  PWME_PWME1=1;//ʹ��
  PWME_PWME3=1;//ʹ��
  PWME_PWME5=1;//ʹ��
  PWME_PWME7=1;//ʹ��
}     
/*************AD��ʼ��*****************/
void AD_INIT(void)
{
  ATD0CTL2 = 0x40;  // AFFC = 1
  ATD0CTL3 = 0x80;  // Right justified; 2 coversions per sequence   10C00    ??wq
  ATD0CTL4 = 0x01;  // 4 ATDCLK as Sample Time; fATDCLK = 0.5*fBUS/(PRS+1) = 20MHz
  ATD0CTL5 = 0x30;  // SCAN = 1; MULT = 1; AN0 as the first A/D channel
}
/*************IO��ʼ��*****************/
void IO_INIT(void)
{
  DDRB_DDRB7=1;//�ҹ����̬�Ŵ����˿� �ߵ�ƽ��Ч
  PORTB_PB7=1;
  DDRB_DDRB6=1;//������̬�Ŵ����˿� �ߵ�ƽ��Ч
  PORTB_PB6=1;
  
  DDRA=0x00;//��ȡ������
  PORTA=0x00;//������
  
  DDRJ_DDRJ7=0;//��������ת��D������
  DDRJ_DDRJ6=0;
  
  DDRE_DDRE6=0;//�ɻɹ�һ��
  PORTE_PE6=1;
  DDRE_DDRE7=0;//�ɻɹܶ���
  PORTE_PE7=1;
  
 // DDRE_DDRE0=0;//���� ��  ʼ��Ϊ0����������
 // DDRE_DDRE1=0;//���� ��
  
  DDRE_DDRE5=1;
  PORTE_PE5=0;
  DDRE_DDRE4=1;         //OLED
  PORTE_PE4=0;
  DDRE_DDRE3=1;
  PORTE_PE3=0;
  DDRE_DDRE2=1;
  PORTE_PE2=0;
  
  DDRB|=0x00;//���뿪��
  PUCR_PUPBE=1;//�����ڲ�����
}
/*************SCI��ʼ��*****************/
void SCI_INIT(void)
 {
  SCI0BD=BUS_CLOCK/16/BAUD;    //������9600,SCI baud rate=SCI bus clock/(16*SBR[12:0])
  SCI0CR1=0x00;
  SCI0CR2=0x08;//�����жϣ������ͣ��������
}
/*************DFLASH��ʼ��*****************/
void DFLASH_INIT(void)
{
   while(FSTAT_CCIF==0);            //�ȴ����ڴ����FLASH�������
   FCLKDIV=0x0F;                    //�ⲿ����Ϊ16M.FLASHʱ�Ӳ�����1M����������ֲ�
   FCNFG=0x00;                      //��ֹ�ж�
   DFPROT_DPOPEN=1;
   while(FCLKDIV_FDIVLD==0);        //�ȴ�ʱ�����óɹ�
}
