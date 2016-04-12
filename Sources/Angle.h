#ifndef _ANGLE_H_
#define _ANGLE_H_

#define Gravity_Offset 1780  //�Ƕ���ƫֵ������ʱȷ��
#define Gravity_Angle_Ratio 5.278  //����ϵ��������ʱȷ��
#define Gyrscope_Offset 1410  //���ٶ���ƫֵ������ʱȷ��
#define Gyrscope_Angle_Ratio 0.67  //����ϵ��������ʱȷ��
#define Turn_Angle_Offset 24  //ת��Ƕ���ƫ
#define Turn_Angle_Ratio 0.67  //����ϵ��
#define Turn_Gravity_Offset 1370

extern void GetAngle(unsigned int *Angle);
extern void AngleCalculate(void);
extern void Angleoffset();

#endif