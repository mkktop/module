/**
 * @file oled.h
 * @author kaukun
 * @brief 
 * @version 1.0
 * @date 2025-01-14
 * 
 * @copyright Copyright (c) 2025
 * 
 * ************************************************************************
 *                               使用说明（必看）
 * 
 * 本模块为0.96寸显示屏的驱动模块，HAL库I2C版本，包含了上下左右滚动，字符字符串，整形，十六进制，二进制
 * 以及反转和各种画图函数,同时也可以调节亮度
 * 本模块采用显存数组，设定好显示内容后需要调用OLED_Update()函数，进行刷新
 * 移植修改宏定义即可
 * 
 * 作者：kaikun
 * 
 */


#ifndef __OLED_H__
#define __OLED_H__

#include "i2c.h"
#include "OLED_Data.h"
#include "string.h"
#include <math.h>
#include <stdio.h>
#include <stdarg.h>
/*参数宏定义*********************/

//如果想使用OLED画图形，请打开此宏定义
#define OLED_Extend 

//根据你使用的I2C来进行修改（同时只能打开一个）
#define I2C hi2c1
//#define I2C hi2c2
//#define I2C hi2c3


//滚动方向宏定义
#define OLED_RIGHT 0x26 //右滚动
#define OLED_LEFT 0x27  //左滚动

#define OLED_VERTICAL_RIGHT 0x29 //垂直和水平右滚动

#define DEV_ADDR  0x78
#define DATA_MODE 0x40
#define CMD_MODE  0x00

#define OLED_8X16				8
#define OLED_6X8				6
/*********************参数宏定义*/


/*函数声明*********************/

// void OLED_WriteData(uint8_t bytes[], uint8_t length);

// void OLED_WriteCmd(uint8_t bytes[], uint8_t length);

/*初始化函数*/

void OLED_Init(void);

/*OLED设置显示指针位置*/

void OLED_SetPointer(uint8_t Page, uint8_t column);

/*OLED更新显示*/

void OLED_Update(void);

/*OLED更新部分显示*/

void OLED_UpdateArea(int16_t X, int16_t Y, uint8_t Width, uint8_t Height);

/*OLED清屏*/

void OLED_Clear(void);

/*OLED部分清屏*/

void OLED_ClearArea(int16_t X, int16_t Y, uint8_t Width, uint8_t Height);

/*OLED全亮*/

void OLED_On(void);

/*OLED显示开启*/

void OLED_Dislay_On(void);

/*OLED显示关闭*/

void OLED_Dislay_Off(void);

/*将OLED显存数组全部取反*/

void OLED_Reverse(void);

/*将OLED显存数组部分取反*/

void OLED_ReverseArea(int16_t X, int16_t Y, uint8_t Width, uint8_t Height);

/*停止滚屏*/

void OLED_Scroll_Stop(void);

/*设置全部区域水平滚动*/

void OLED_Horizontal_Scroll(uint8_t dir);

/*设置部分区域水平滚动*/

void OLED_Horizontal_Scroll_Area(uint8_t dir,uint8_t start,uint8_t end);

/*设置水平与垂直滚动滚动*/

void OLED_Horizontal_And_Vertical_Scroll(uint8_t dir);

/*设置显示亮度*/

void OLED_Bright_Set(uint8_t Bright);

/*OLED显示图像*/

void OLED_ShowImage(int16_t X, int16_t Y, uint8_t Width, uint8_t Height, const uint8_t *Image);

/*OLED显示字符*/

void OLED_ShowChar(int16_t X, int16_t Y, char Char, uint8_t FontSize);

/*OLED显示字符串*/

void OLED_ShowString(int16_t X, int16_t Y, char *String, uint8_t FontSize);

/*次方函数*/

uint32_t OLED_Pow(uint32_t X, uint32_t Y);

/*OLED显示数字*/

void OLED_ShowNum(int16_t X, int16_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize);

/*OLED显示有符号数字（十进制，整数）*/

void OLED_ShowSignedNum(int16_t X, int16_t Y, int32_t Number, uint8_t Length, uint8_t FontSize);

/*OLED显示十六进制整数*/

void OLED_ShowHexNum(int16_t X, int16_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize);

/*显示二进制数字*/

void OLED_ShowBinNum(int16_t X, int16_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize);

/*OLED显示浮点数*/

void OLED_ShowFloatNum(int16_t X, int16_t Y, double Number, uint8_t IntLength, uint8_t FraLength, uint8_t FontSize);

/*OLED_printf函数*/
void OLED_Printf(int16_t X, int16_t Y, uint8_t FontSize, char *format, ...);

//OLED在指定位置画一个点

void OLED_DrawPoint(int16_t X, int16_t Y);

//OLED获取指定位置点的值

uint8_t OLED_GetPoint(int16_t X, int16_t Y);

//OLED画线

void OLED_DrawLine(int16_t X0, int16_t Y0, int16_t X1, int16_t Y1);

//OLED矩形

void OLED_DrawRectangle(int16_t X, int16_t Y, uint8_t Width, uint8_t Height, uint8_t IsFilled);

//OLED三角形

void OLED_DrawTriangle(int16_t X0, int16_t Y0, int16_t X1, int16_t Y1, int16_t X2, int16_t Y2, uint8_t IsFilled);


//OLED圆形

void OLED_DrawCircle(int16_t X, int16_t Y, uint8_t Radius, uint8_t IsFilled);

//OLED椭圆

void OLED_DrawEllipse(int16_t X, int16_t Y, uint8_t A, uint8_t B, uint8_t IsFilled);

//OLED画圆弧

void OLED_DrawArc(int16_t X, int16_t Y, uint8_t Radius, int16_t StartAngle, int16_t EndAngle, uint8_t IsFilled);


#endif /* __OLED_H__ */