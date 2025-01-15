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
 *                               ʹ��˵�����ؿ���
 * 
 * ��ģ��Ϊ0.96����ʾ��������ģ�飬HAL��I2C�汾���������������ҹ������ַ��ַ��������Σ�ʮ�����ƣ�������
 * �Լ���ת�͸��ֻ�ͼ����,ͬʱҲ���Ե�������
 * ��ģ������Դ����飬�趨����ʾ���ݺ���Ҫ����OLED_Update()����������ˢ��
 * ��ֲ�޸ĺ궨�弴��
 * 
 * ���ߣ�kaikun
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
/*�����궨��*********************/

//�����ʹ��OLED��ͼ�Σ���򿪴˺궨��
#define OLED_Extend 

//������ʹ�õ�I2C�������޸ģ�ͬʱֻ�ܴ�һ����
#define I2C hi2c1
//#define I2C hi2c2
//#define I2C hi2c3


//��������궨��
#define OLED_RIGHT 0x26 //�ҹ���
#define OLED_LEFT 0x27  //�����

#define OLED_VERTICAL_RIGHT 0x29 //��ֱ��ˮƽ�ҹ���

#define DEV_ADDR  0x78
#define DATA_MODE 0x40
#define CMD_MODE  0x00

#define OLED_8X16				8
#define OLED_6X8				6
/*********************�����궨��*/


/*��������*********************/

// void OLED_WriteData(uint8_t bytes[], uint8_t length);

// void OLED_WriteCmd(uint8_t bytes[], uint8_t length);

/*��ʼ������*/

void OLED_Init(void);

/*OLED������ʾָ��λ��*/

void OLED_SetPointer(uint8_t Page, uint8_t column);

/*OLED������ʾ*/

void OLED_Update(void);

/*OLED���²�����ʾ*/

void OLED_UpdateArea(int16_t X, int16_t Y, uint8_t Width, uint8_t Height);

/*OLED����*/

void OLED_Clear(void);

/*OLED��������*/

void OLED_ClearArea(int16_t X, int16_t Y, uint8_t Width, uint8_t Height);

/*OLEDȫ��*/

void OLED_On(void);

/*OLED��ʾ����*/

void OLED_Dislay_On(void);

/*OLED��ʾ�ر�*/

void OLED_Dislay_Off(void);

/*��OLED�Դ�����ȫ��ȡ��*/

void OLED_Reverse(void);

/*��OLED�Դ����鲿��ȡ��*/

void OLED_ReverseArea(int16_t X, int16_t Y, uint8_t Width, uint8_t Height);

/*ֹͣ����*/

void OLED_Scroll_Stop(void);

/*����ȫ������ˮƽ����*/

void OLED_Horizontal_Scroll(uint8_t dir);

/*���ò�������ˮƽ����*/

void OLED_Horizontal_Scroll_Area(uint8_t dir,uint8_t start,uint8_t end);

/*����ˮƽ�봹ֱ��������*/

void OLED_Horizontal_And_Vertical_Scroll(uint8_t dir);

/*������ʾ����*/

void OLED_Bright_Set(uint8_t Bright);

/*OLED��ʾͼ��*/

void OLED_ShowImage(int16_t X, int16_t Y, uint8_t Width, uint8_t Height, const uint8_t *Image);

/*OLED��ʾ�ַ�*/

void OLED_ShowChar(int16_t X, int16_t Y, char Char, uint8_t FontSize);

/*OLED��ʾ�ַ���*/

void OLED_ShowString(int16_t X, int16_t Y, char *String, uint8_t FontSize);

/*�η�����*/

uint32_t OLED_Pow(uint32_t X, uint32_t Y);

/*OLED��ʾ����*/

void OLED_ShowNum(int16_t X, int16_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize);

/*OLED��ʾ�з������֣�ʮ���ƣ�������*/

void OLED_ShowSignedNum(int16_t X, int16_t Y, int32_t Number, uint8_t Length, uint8_t FontSize);

/*OLED��ʾʮ����������*/

void OLED_ShowHexNum(int16_t X, int16_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize);

/*��ʾ����������*/

void OLED_ShowBinNum(int16_t X, int16_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize);

/*OLED��ʾ������*/

void OLED_ShowFloatNum(int16_t X, int16_t Y, double Number, uint8_t IntLength, uint8_t FraLength, uint8_t FontSize);

/*OLED_printf����*/
void OLED_Printf(int16_t X, int16_t Y, uint8_t FontSize, char *format, ...);

//OLED��ָ��λ�û�һ����

void OLED_DrawPoint(int16_t X, int16_t Y);

//OLED��ȡָ��λ�õ��ֵ

uint8_t OLED_GetPoint(int16_t X, int16_t Y);

//OLED����

void OLED_DrawLine(int16_t X0, int16_t Y0, int16_t X1, int16_t Y1);

//OLED����

void OLED_DrawRectangle(int16_t X, int16_t Y, uint8_t Width, uint8_t Height, uint8_t IsFilled);

//OLED������

void OLED_DrawTriangle(int16_t X0, int16_t Y0, int16_t X1, int16_t Y1, int16_t X2, int16_t Y2, uint8_t IsFilled);


//OLEDԲ��

void OLED_DrawCircle(int16_t X, int16_t Y, uint8_t Radius, uint8_t IsFilled);

//OLED��Բ

void OLED_DrawEllipse(int16_t X, int16_t Y, uint8_t A, uint8_t B, uint8_t IsFilled);

//OLED��Բ��

void OLED_DrawArc(int16_t X, int16_t Y, uint8_t Radius, int16_t StartAngle, int16_t EndAngle, uint8_t IsFilled);


#endif /* __OLED_H__ */