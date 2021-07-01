#ifndef _OLED_H_
#define _OLED_H_
#include"stdio.h"

//�ܽŶ��壻
#define byte  unsigned char
#define word  unsigned int
#define dword unsigned long

#define DC_CLR P2OUT &=~BIT0
#define DC_SET P2OUT |=BIT0

#define RST_CLR P2OUT &=~BIT1
#define RST_SET P2OUT |=BIT1

#define SDA_CLR P2OUT &=~BIT2
#define SDA_SET P2OUT |=BIT2

#define SCL_CLR P2OUT &=~BIT3
#define SCL_SET P2OUT |=BIT3

#define X_WIDTH 128
#define Y_WIDTH 64

//��������

void delay(int ms);//��ʱ
void LCD_WrCmd(unsigned char cmd);
void LCD_WrDat(unsigned char dat);
void LCD_Init(void);
void LCD_CLS(void);
void LCD_P6x8Str(byte x,byte y,byte ch[]);
void LCD_P8x16Str(byte x,byte y,byte *ch);
void LCD_Fill(byte dat);

#endif
