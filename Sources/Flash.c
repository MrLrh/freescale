#include "Include.h"

uint_16 Buffer[3];
/*************************************************************/
/*                     ��DFLASHд������                      */
/*************************************************************/
void Dflash_Write(uint_16 Address16,uchar_8 i) 
{
    while(FSTAT_CCIF==0); 
    if(FSTAT_ACCERR)           //�жϲ������־λ��
        FSTAT_ACCERR=1;
    if(FSTAT_FPVIOL)           //�жϲ������־λ��
        FSTAT_FPVIOL=1;
    FCCOBIX_CCOBIX=0x00; 
    FCCOB=0x1110;         //д������͸�λ��ַ
    FCCOBIX_CCOBIX=0x01;  //��ַ��16λ
    FCCOB=Address16+i*6;         //д���16λ��ַ
    FCCOBIX_CCOBIX=0x02;  //д���һ������
    FCCOB=Buffer[0];
    FCCOBIX_CCOBIX=0x03;  //д��ڶ�������
    FCCOB=Buffer[1];
    FCCOBIX_CCOBIX=0x04;  //д�����������
    FCCOB=Buffer[2];
    /*FCCOBIX_CCOBIX=0x05;  //д����ĸ�����
    FCCOB=Buffer[3];      */
      
    FSTAT_CCIF=1;         //д��ִ������
    while(FSTAT_CCIF==0); //�ȴ�ִ�����
}   
/*************************************************************/
/*                     ��DFLASH��ȡ����                      */
/*************************************************************/
uint_16 Dflash_Read(uint_16 destination) 
{
    byte   lastepage;          //���ڴ洢EPAGE��ֵ
    byte   epage;              //���ڼ���EPAGE��ֵ
    unsigned int data;         //��ȡ��������

    lastepage = EPAGE;   //����EPAGE��ֵ

    epage = (byte)((DFLASH_START_PAGE)+(destination >>10));   //����EPAGE
    EPAGE=epage;                                                     //��EPAGE��ֵ
 
    data = READword((destination & (DFLASH_PAGE_SIZE - 1)) + DFLASH_PAGE_WINDOW_SIZE);  //��ȡҳ�洰���е�����

    EPAGE= lastepage;       //�ָ�EPAGE��ֵ

    return(data);  
}
/*************************************************************/
/*                    ����ȫ��DFLASH                 */
/*************************************************************/
void Dflash_Erase(uint_16 Address16)
{
  while(FSTAT_CCIF==0);
  if(FSTAT_ACCERR)           //�жϲ������־λ��
      FSTAT_ACCERR=1;
  if(FSTAT_FPVIOL)           //�жϲ������־λ��
      FSTAT_FPVIOL=1;
  
  FCCOBIX_CCOBIX=0x00;
  FCCOB=0x0910;           //д���������͸�λ��ַ
  FCCOBIX_CCOBIX=0x01;   
  FCCOB=Address16;           //д���16λ�ĵ�ַ
  FSTAT_CCIF=1;           //����ִ������
  while(FSTAT_CCIF==0);   //�ȴ�ִ�����
}


