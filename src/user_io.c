/*
 * user_io.c
 *
 *  Created on: 2020��10��10��
 *      Author: gxk
 */

#include "oled.h"
#include "user_io.h"
#include "drive.h"
#include "sensor.h"
#include <msp430.h>

void init_oled(void)
{
    P2DIR |= BIT0 + BIT1 + BIT2 + BIT3;
    LCD_Init();
}

int max_pwm = 0;
void show()
{
    switch (mode)
    {
    case 0:
        LCD_P6x8Str(4, 1, "CHOOSE TIME ");
        break;
    case 1:
        LCD_P6x8Str(4, 1, "CHOOSE ANGLE");
        break;
    case 2:
        LCD_P6x8Str(4, 1, "PREPARE     ");
        break;
    case 3:
        LCD_P6x8Str(4, 1, "RUN         ");
        break;
    case 4:
        LCD_P6x8Str(4, 1, "PAUSE       ");
        break;
    }

    unsigned char setting_str[] = "TIME: XX  ANGLE: XX";
    setting_str[6] = '0' + time / 10;
    setting_str[7] = '0' + time % 10;
    setting_str[17] = '0' + angle / 10;
    setting_str[18] = '0' + angle % 10;
    LCD_P6x8Str(4, 3, setting_str);

    unsigned char info_str[] = "S: XXX  T: XX.X";
    int x = realJourney / 480;
    info_str[3] = '0' + x % 1000 / 100;
    info_str[4] = '0' + x % 100 / 10;
    info_str[5] = '0' + x % 10;
    int y = realTime / 2;
    info_str[11] = '0' + y % 1000 / 100;
    info_str[12] = '0' + y % 100 / 10;
    info_str[14] = '0' + y % 10;
    LCD_P6x8Str(4, 5, info_str);

    unsigned char sensor_str[] = "SENSOR: XXXXX XXXXX";
    sensor_str[8] = '0' + !!S0;
    sensor_str[9] = '0' + !!S1;
    sensor_str[10] = '0' + !!S2;
    sensor_str[11] = '0' + !!S3;
    sensor_str[12] = '0' + !!S4;
    sensor_str[14] = '0' + !!S5;
    sensor_str[15] = '0' + !!S6;
    sensor_str[16] = '0' + !!S7;
    sensor_str[17] = '0' + !!S8;
    sensor_str[18] = '0' + !!S9;
    LCD_P6x8Str(4, 7, sensor_str);

    //    unsigned char speed_str[] = "SPEED: XXX XXX";
    //    speed_str[7] = '0' + left_speed%1000/100;
    //    speed_str[8] = '0' + left_speed%100/10;
    //    speed_str[9] = '0' + left_speed%10;
    //    speed_str[11] = '0' + right_speed%1000/100;
    //    speed_str[12] = '0' + right_speed%100/10;
    //    speed_str[13] = '0' + right_speed%10;
    //    LCD_P6x8Str(4,5,speed_str);

    ////////////////
    //    pid_str[0] = '0' + !!S0;
    //    pid_str[1] = '0' + !!S1;
    //    pid_str[2] = '0' + !!S2;
    //    pid_str[3] = '0' + !!S3;
    //    pid_str[4] = '0' + !!S4;
    //    pid_str[5] = '0' + !!S5;
    //    pid_str[6] = '0' + !!S6;
    //    pid_str[7] = '0' + !!S7;
    //    pid_str[8] = ' ';
    //    pid_str[9] = ' ';
    //////////////////
    //    LCD_P6x8Str(4,7,pid_str);
}

void init_key(void)
{
    P2DIR |= BIT7;
    P2OUT &= ~BIT7;
    P8DIR |= BIT0;
    P8OUT &= ~BIT0;

    P2DIR &= ~BIT6;
    P2SEL &= ~BIT6;
    P2REN |= BIT6;
    P2OUT |= BIT6;
    P2IE |= BIT6;
    P2IES |= BIT6;
    P2IFG = 0;

    P3DIR |= BIT1;
    P3SEL &= ~BIT1;
    P3REN |= BIT1;
    P3OUT |= BIT1;
}

int get_key(void)
{
    int i = 16;
    if (!(P3IN & BIT1))
    {
        while (--i)
            __delay_cycles(32768);
        if (!(P3IN & BIT1))
        {
            while (!(P3IN & BIT1))
                ;
            return 1;
        }
        return 0;
    }
    return 0;
}

// �����ж�
#pragma vector = PORT2_VECTOR
__interrupt void Port2(void)
{
    int i = 4;
    if (P2IFG & BIT6)
    {
        while (--i)
            __delay_cycles(32768);
        if (P2IFG & BIT6)
        {
            while (!(P2IN & BIT6))
                ;
            i = 4;
            while (i--)
                __delay_cycles(32768)
            mode == 4 ? mode = 0 : mode++;
            P2OUT ^= BIT7;
        }
    }
    P2IFG = 0;
}

//#pragma vector = TIMER1_A1_VECTOR
//__interrupt void Timer1_A1 (void)
//{
//    static int this_key = 0;
//    int last_key = 0;
//
//    TA1CCR1 += 326;
//    if (TA1IV & 0x0E == 2)
//    {
//        last_key = this_key;
//
//        if (P2IN & BIT6)
//        {
//            this_key = 0;
//        }
//        else
//        {
//            this_key = 1;
//        }
//        if (this_key==0 && last_key==1)
//        {
//                        mode==5?mode=0:mode++;
//                                    P2OUT ^= BIT7;
//                                    P8OUT ^= BIT0;
//        }
////        if (!(P2IN & BIT6))
////        {
////            last_key = 1;
////        }
////        else if (last_key && (P2IN & BIT6))
////        {
////            mode==5?mode=0:mode++;
////                        P2OUT ^= BIT7;
////                        P8OUT ^= BIT0;
////            last_key = 0;
////        }
//    }
//}
