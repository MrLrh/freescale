#include "Include.h"

uint_16 Buffer[3];
/*************************************************************/
/*                     向DFLASH写入数据                      */
/*************************************************************/
void Dflash_Write(uint_16 Address16,uchar_8 i) 
{
    while(FSTAT_CCIF==0); 
    if(FSTAT_ACCERR)           //判断并清除标志位；
        FSTAT_ACCERR=1;
    if(FSTAT_FPVIOL)           //判断并清除标志位；
        FSTAT_FPVIOL=1;
    FCCOBIX_CCOBIX=0x00; 
    FCCOB=0x1110;         //写入命令和高位地址
    FCCOBIX_CCOBIX=0x01;  //地址后16位
    FCCOB=Address16+i*6;         //写入低16位地址
    FCCOBIX_CCOBIX=0x02;  //写入第一个数据
    FCCOB=Buffer[0];
    FCCOBIX_CCOBIX=0x03;  //写入第二个数据
    FCCOB=Buffer[1];
    FCCOBIX_CCOBIX=0x04;  //写入第三个数据
    FCCOB=Buffer[2];
    /*FCCOBIX_CCOBIX=0x05;  //写入第四个数据
    FCCOB=Buffer[3];      */
      
    FSTAT_CCIF=1;         //写入执行命令
    while(FSTAT_CCIF==0); //等待执行完毕
}   
/*************************************************************/
/*                     由DFLASH读取数据                      */
/*************************************************************/
uint_16 Dflash_Read(uint_16 destination) 
{
    byte   lastepage;          //用于存储EPAGE的值
    byte   epage;              //用于计算EPAGE的值
    unsigned int data;         //读取出的数据

    lastepage = EPAGE;   //保存EPAGE的值

    epage = (byte)((DFLASH_START_PAGE)+(destination >>10));   //计算EPAGE
    EPAGE=epage;                                                     //给EPAGE赋值
 
    data = READword((destination & (DFLASH_PAGE_SIZE - 1)) + DFLASH_PAGE_WINDOW_SIZE);  //读取页面窗口中的数据

    EPAGE= lastepage;       //恢复EPAGE的值

    return(data);  
}
/*************************************************************/
/*                    擦除全部DFLASH                 */
/*************************************************************/
void Dflash_Erase(uint_16 Address16)
{
  while(FSTAT_CCIF==0);
  if(FSTAT_ACCERR)           //判断并清除标志位；
      FSTAT_ACCERR=1;
  if(FSTAT_FPVIOL)           //判断并清除标志位；
      FSTAT_FPVIOL=1;
  
  FCCOBIX_CCOBIX=0x00;
  FCCOB=0x0910;           //写入擦除命令和高位地址
  FCCOBIX_CCOBIX=0x01;   
  FCCOB=Address16;           //写入低16位的地址
  FSTAT_CCIF=1;           //启动执行命令
  while(FSTAT_CCIF==0);   //等待执行完成
}


