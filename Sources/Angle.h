#ifndef _ANGLE_H_
#define _ANGLE_H_

<<<<<<< HEAD
#define Gravity_Offset 1780  //角度零偏值，调试时确定
#define Gravity_Angle_Ratio 5.278  //比例系数，调试时确定
#define Gyrscope_Offset 1410  //角速度零偏值，调试时确定
#define Gyrscope_Angle_Ratio 0.67  //比例系数，调试时确定
#define Turn_Angle_Offset 24  //转向角度零偏
#define Turn_Angle_Ratio 0.67  //比例系数
#define Turn_Gravity_Offset 1370
=======
#define Gravity_Offset 1788  //角度零偏值，调试时确定
#define Gravity_Angle_Ratio 5.278  //比例系数，调试时确定
#define Gyrscope_Offset 1408  //角速度零偏值，调试时确定
#define Gyrscope_Angle_Ratio 0.67  //比例系数，调试时确定
#define Turn_Angle_Offset 24  //转向角度零偏
#define Turn_Angle_Ratio 0.67  //比例系数
#define Turn_Gravity_Offset 1288
>>>>>>> 60d881f1213707a13672e023a0d870fa7f13b89f

extern void GetAngle(unsigned int *Angle);
extern void AngleCalculate(void);

#endif