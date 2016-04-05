#ifndef _DFLASH_H_
#define _DFLASH_H_

#define READword(address)    ((unsigned int)(*(volatile unsigned int *__near)(address))) 
#define DFLASH_START_PAGE  0x00  //定义DFLASH的起始页
#define DFLASH_START_ADDRESS 0x100000  //定义DFLASH的起始地址
#define DFLASH_PAGE_SIZE  0x0400  //定义DFLASH的大小为1K
#define DFLASH_PAGE_WINDOW_SIZE 0x800 //定义DFLASH的页面窗口大小
#define data_Address 0x0000

extern void Dflash_Write(uint_16 Address16,uchar_8 i);
extern uint_16 Dflash_Read(uint_16 destination);
extern void Dflash_Erase(uint_16 Address16);

#endif