#ifndef _DFLASH_H_
#define _DFLASH_H_

#define READword(address)    ((unsigned int)(*(volatile unsigned int *__near)(address))) 
#define DFLASH_START_PAGE  0x00  //����DFLASH����ʼҳ
#define DFLASH_START_ADDRESS 0x100000  //����DFLASH����ʼ��ַ
#define DFLASH_PAGE_SIZE  0x0400  //����DFLASH�Ĵ�СΪ1K
#define DFLASH_PAGE_WINDOW_SIZE 0x800 //����DFLASH��ҳ�洰�ڴ�С
#define data_Address 0x0000

extern void Dflash_Write(uint_16 Address16,uchar_8 i);
extern uint_16 Dflash_Read(uint_16 destination);
extern void Dflash_Erase(uint_16 Address16);

#endif