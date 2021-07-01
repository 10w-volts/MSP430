//12864.c
#include"msp430.h"
#include"oled.h"
#include"oled_font.h"
#include"stdio.h"

void delay(int ms)//��ʱ
{
   int ii,jj;
   if (ms<1) ms=1;
   for(ii=0;ii<ms;ii++)
   for(jj=0;jj<500;jj++);

}

//��LCDд���ַ
void LCD_WrDat(unsigned char dat)
{
   unsigned char i=8;
     DC_SET;
     SCL_CLR;
  while(i--)
  {
    if(dat&0x80)
      {SDA_SET;}
    else
      {SDA_CLR;}
        SCL_SET ;
        SCL_CLR;
        dat<<=1;
  }
}

//��LCD����д������
void LCD_WrCmd(unsigned char cmd)
{
    unsigned char i=8;
    DC_CLR;
    SCL_CLR;
  while(i--)
  {
    if(cmd&0x80)
    {SDA_SET;}
    else{SDA_CLR;}
    SCL_SET;
    SCL_CLR;
    cmd<<=1;
  }
}

//��ʱ����
void LCD_DLY_ms(unsigned int ms)
{                         
  unsigned int a;
  while(ms)
  {
    a=1335;
    while(a--);
    ms--;
  }
  return;
}

//LCD������
void LCD_CLS(void)
{
    unsigned char y,x;
    for(y=0;y<8;y++)
    {
        LCD_WrCmd(0xb0+y);
        LCD_WrCmd(0x01);
        LCD_WrCmd(0x10);
        for(x=0;x<X_WIDTH;x++)
            LCD_WrDat(0);
    }
}

//��������
void LCD_Fill(unsigned char bmp_dat)
{
    unsigned char y,x;

    for(y=0;y<8;y++)
    {
        LCD_WrCmd(0xb0+y);
        LCD_WrCmd(0x01);
        LCD_WrCmd(0x10);
        for(x=0;x<X_WIDTH;x++)
            LCD_WrDat(bmp_dat);
    }
}

void LCD_Set_Pos(unsigned char x, unsigned char y)//��������
{
  LCD_WrCmd(0xb0+y);
  LCD_WrCmd(((x&0xf0)>>4)|0x10);
  LCD_WrCmd((x&0x0f)|0x00);
}

//LCDҺ������ʼ��
void LCD_Init(void)
{
        SCL_SET;
        RST_CLR;
    LCD_DLY_ms(50);
        RST_SET;

    //���ϵ絽���濪ʼ��ʼ��Ҫ���㹻��ʱ�䣬���ȴ�RC��λ���

  LCD_WrCmd(0xae);//--turn off oled panel
  LCD_WrCmd(0x00);//---set low column address
  LCD_WrCmd(0x10);//---set high column address
  LCD_WrCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
  LCD_WrCmd(0x81);//--set contrast control register
  LCD_WrCmd(0xcf); // Set SEG Output Current Brightness
  LCD_WrCmd(0xa1);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
  LCD_WrCmd(0xc8);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
  LCD_WrCmd(0xa6);//--set normal display
  LCD_WrCmd(0xa8);//--set multiplex ratio(1 to 64)
  LCD_WrCmd(0x3f);//--1/64 duty
  LCD_WrCmd(0xd3);//-set display offset Shift Mapping RAM Counter (0x00~0x3F)
  LCD_WrCmd(0x00);//-not offset
  LCD_WrCmd(0xd5);//--set display clock divide ratio/oscillator frequency
  LCD_WrCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
  LCD_WrCmd(0xd9);//--set pre-charge period
  LCD_WrCmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
  LCD_WrCmd(0xda);//--set com pins hardware configuration
  LCD_WrCmd(0x12);
  LCD_WrCmd(0xdb);//--set vcomh
  LCD_WrCmd(0x40);//Set VCOM Deselect Level
  LCD_WrCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
  LCD_WrCmd(0x02);//
  LCD_WrCmd(0x8d);//--set Charge Pump enable/disable
  LCD_WrCmd(0x14);//--set(0x10) disable
  LCD_WrCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
  LCD_WrCmd(0xa6);// Disable Inverse Display On (0xa6/a7)
  LCD_WrCmd(0xaf);//--turn on oled panel
  LCD_Fill(0x00);  //��ʼ����
  LCD_Set_Pos(0,0);
} 

//==============================================================
//��������LCD_P6x8Str(unsigned char x,unsigned char y,unsigned char *p)
//����������д��һ���׼ASCII�ַ���
//��������ʾ��λ�ã�x,y����yΪҳ��Χ0��7��Ҫ��ʾ���ַ���
//���أ���
//==============================================================
void LCD_P6x8Str(unsigned char x,unsigned char y,unsigned char ch[])
{
  unsigned char c=0,i=0,j=0;
  while (ch[j]!='\0')
  {
    c =ch[j]-32;
    if(x>126){x=0;y++;}
    LCD_Set_Pos(x,y);
    for(i=0;i<6;i++)
      LCD_WrDat(F6x8[c][i]);
    x+=6;
    j++;
  }
}

void LCD_P8x16Str(unsigned char x,unsigned char y,unsigned char *ch)
{
  unsigned char c=0,i=0,j=0;

  while (ch[j]!='\0')
  {
    c =ch[j]-32;
    if(x>120){x=0;y++;}
    LCD_Set_Pos(x,y);
    for(i=0;i<8;i++)
      LCD_WrDat(F8X16[c*16+i]);
    LCD_Set_Pos(x,y+1);
    for(i=0;i<8;i++)
      LCD_WrDat(F8X16[c*16+i+8]);
    x+=8;
    j++;
  }
}
