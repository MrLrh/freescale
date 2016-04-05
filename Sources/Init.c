#include "Include.h" 

#define BUS_CLOCK		   60000000	    //总线频率
#define OSC_CLOCK		   16000000	    //晶振频率
#define BAUD 9600                   //串口波特率

/*************锁相环初始化*****************/
void PLL_INIT(void) 
{
   
    //clksel.7(PLLSEL选择位)决定内部总线时钟来源  =0,BusClock=OSCCLK/2;   =1,BusClock=PLLCLK/2
   CLKSEL_PLLSEL=0;	  
           
   PLLCTL_PLLON=0;  //PLLCTL.6(pllon)设为0;先关闭PLL

    //根据需要的时钟频率设置SYNR和REFDV寄存器  BusClock=PLLCLK/2;
    //计算公式: FVOC=2*OSCCLK*((SYNR+1)/(REFDV+1)) 
    //          PLLCLK=FVCO/(2*POSTDIV) 
    //          BusClock=PLLCLK/2
    //SYNR(.7.6)  32MHZ<FVOC<48MHZ   00   0
    //SYNR(.7.6)  48MHZ<FVOC<80MHZ   01   4
    //SYNR(.7.6)  80MHZ<FVOC<120MHZ  10   8
    //SYNR(.7.6)  120MHZ<FVOC        11   C		
   SYNR=0xC0|0x0E; 	
    //计算公式 FREF=OSCCLK/(REDIV*2)	
    //REFDV(.7.6)  1MHZ<FREF<2MHZ    00  0
    //REFDV(.7.6)  2MHZ<FREF<6MHZ    01  4
    //REFDV(.7.6)  6MHZ<FREF<12MHZ   10  8
    //REFDV(.7.6)  12MHZ<FREF	       11  C
   REFDV=0x40|0x03;
 
   
   
  //后分配寄存器PLLCLK=FVCO/(2*POSTDIV)   如果POSTDIV=0x00，PLLCLK=FVCO             
   POSTDIV=0x00;     

   PLLCTL_PLLON=1;  //PLLCTL.6(pllon)设为1;打开PLL

   _asm(nop);         
   _asm(nop);
   while(CRGFLG_LOCK==0); //根据CRGFLG寄存器的LOCK位，确定PLL是否稳定	 LOCK==1 稳定，==0 不稳定  
   CLKSEL_PLLSEL =1;     //选择PLL作为时钟源		       

}
/*************定时器中断初始化*****************/    
void PIT_INIT(void) 
{
  PITCFLMT_PITE=0;//定时通道关闭
  PITCE_PCE0=1;//定时器通道0使能
  PITMTLD0=120-1;//8位定时器初值
  PITLD0=500-1;//16位定时器初值       1ms AD定时中断
  PITINTE_PINTE0=1;//定时器中断通道0中断使能
 
 /* PITCE_PCE1=1;//定时器通道1使能
  PITLD1=5000-1;//16位定时器初值        10ms  读取速度中断
  PITINTE_PINTE1=1;//定时器中断通道1中断使能
  
  PITCE_PCE2=1;//定时器通道2使能
  PITLD2=2500-1;//16位定时器初值        5ms 控制中断
  PITINTE_PINTE2=1;//定时器中断通道2中断使能   */
  PITCFLMT_PITE=1;//定时器通道0使能 
}
/*************PWM初始化*****************/
void PWM_INIT(void)
{
  PWME=0x00;//禁止PWM
  PWMPRCLK=0x00;//时钟预分频寄存器设置 A=60M,B=60M
  PWMSCLA=0x02;//SA=A/2/2=15M    时钟设置
  PWMSCLB=0x02;//SB=B/2/2=15M    时钟设置
  PWMCTL_CON01=1;//0,1通道级联
  PWMCTL_CON23=1;//2,3通道级联
  PWMCTL_CON45=1;//4,5通道级联
  PWMCTL_CON67=1;//6,7通道级联
  PWMCLK_PCLK1=1;// PWM01-----SA             时钟源的选择
  PWMPOL_PPOL1=1;//对外输出波形先是高电平然后再变为低电平
  PWMCLK_PCLK3=1;// PWM23-----SB             时钟源的选择
  PWMPOL_PPOL3=1;//对外输出波形先是高电平然后再变为低电平
  PWMCLK_PCLK5=1;// PWM45-----SA             时钟源的选择
  PWMPOL_PPOL5=1;//对外输出波形先是高电平然后再变为低电平
  PWMCLK_PCLK7=1;// PWM67-----SB             时钟源的选择
  PWMPOL_PPOL7=1;//对外输出波形先是高电平然后再变为低电平
  PWMCAE_CAE1=0;//左对齐 对齐方式设置
  PWMCAE_CAE3=0;//左对齐 对齐方式设置
  PWMCAE_CAE5=0;//左对齐 对齐方式设置
  PWMCAE_CAE7=0;//左对齐 对齐方式设置
  PWMPER01=5000;//频率=SA/5000 3K    左电机正转
  PWMDTY01=0;//占空比为0
  PWMPER23=5000;//频率=SB/5000 3K    左电机反转
  PWMDTY23=0;//占空比为0
  PWMPER45=5000;//频率=SA/5000 3K    右电机正转
  PWMDTY45=0;//占空比为0
  PWMPER67=5000;//频率=SB/5000 3K    右电机反转      
  PWMDTY67=0;//占空比为0
  PWME_PWME1=1;//使能
  PWME_PWME3=1;//使能
  PWME_PWME5=1;//使能
  PWME_PWME7=1;//使能
}     
/*************AD初始化*****************/
void AD_INIT(void)
{
  ATD0CTL2 = 0x40;  // AFFC = 1
  ATD0CTL3 = 0x80;  // Right justified; 2 coversions per sequence   10C00    ??wq
  ATD0CTL4 = 0x01;  // 4 ATDCLK as Sample Time; fATDCLK = 0.5*fBUS/(PRS+1) = 20MHz
  ATD0CTL5 = 0x30;  // SCAN = 1; MULT = 1; AN0 as the first A/D channel
}
/*************IO初始化*****************/
void IO_INIT(void)
{
  DDRB_DDRB7=1;//右光编三态门触发端口 高电平无效
  PORTB_PB7=1;
  DDRB_DDRB6=1;//左光编三态门触发端口 高电平无效
  PORTB_PB6=1;
  
  DDRA=0x00;//读取计数器
  PORTA=0x00;//先清零
  
  DDRJ_DDRJ7=0;//右轮正反转，D触发器
  DDRJ_DDRJ6=0;
  
  DDRE_DDRE6=0;//干簧管一号
  PORTE_PE6=1;
  DDRE_DDRE7=0;//干簧管二号
  PORTE_PE7=1;
  
 // DDRE_DDRE0=0;//按键 上  始终为0，不用设置
 // DDRE_DDRE1=0;//按键 下
  
  DDRE_DDRE5=1;
  PORTE_PE5=0;
  DDRE_DDRE4=1;         //OLED
  PORTE_PE4=0;
  DDRE_DDRE3=1;
  PORTE_PE3=0;
  DDRE_DDRE2=1;
  PORTE_PE2=0;
  
  DDRB|=0x00;//拨码开关
  PUCR_PUPBE=1;//采用内部上拉
}
/*************SCI初始化*****************/
void SCI_INIT(void)
 {
  SCI0BD=BUS_CLOCK/16/BAUD;    //波特率9600,SCI baud rate=SCI bus clock/(16*SBR[12:0])
  SCI0CR1=0x00;
  SCI0CR2=0x08;//允许中断，允许发送，允许接受
}
/*************DFLASH初始化*****************/
void DFLASH_INIT(void)
{
   while(FSTAT_CCIF==0);            //等待正在处理的FLASH操作完成
   FCLKDIV=0x0F;                    //外部晶振为16M.FLASH时钟不超过1M，具体参照手册
   FCNFG=0x00;                      //禁止中断
   DFPROT_DPOPEN=1;
   while(FCLKDIV_FDIVLD==0);        //等待时钟设置成功
}
