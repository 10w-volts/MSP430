/*
 * drive.c
 *
 *  Created on: 2020骞�10鏈�10鏃�
 *      Author: cjp
 */

#include <msp430.h>
#include "drive.h"
#include "sensor.h"
#include "math.h"

int woc = 1;

int mode = 0;
unsigned int time = 20;
unsigned int angle = 0;

unsigned long tarJourney = 0; // 鎬昏矾绋�1m锛�1x1200x20
int tarTime = 0;              // 鎬绘椂闂�10s锛�10x20
unsigned long realJourney = 0;
int realTime = 0;

int P_coeff = 2; // PID鎺у埗涓殑绯绘暟
int I_coeff_div = 2;
int D_coeff = 4;

int turn_angle = 0;

int pwm_min = 50 * 25; // 鏈�灏弍wm
int pwm_max = 68 * 25; // 鏈�澶wm

unsigned int Tar_speedL = 0;
unsigned int Tar_speedR = 0;

unsigned int left_speed = 0;
unsigned int right_speed = 0;

unsigned int left_count = 0;
unsigned int right_count = 0;

void init_drive(void)
{
    P8DIR |= BIT1 + BIT3;
    P8OUT &= ~(BIT1 + BIT3);

    // pwm娉㈡ā鍧�5涓彛锛�4涓�10kHz锛�1涓�50Hz
    P4DIR |= BIT2 + BIT3 + BIT4 + BIT5; // 閫夋嫨4.2銆�4.3銆�4.4銆�4.5寮曡剼杈撳嚭10kHzpwm
    P4SEL |= BIT2 + BIT3 + BIT4 + BIT5;
    TB0CTL |= TASSEL_2 + MC_1 + TACLR; // 閫夋嫨SMCLK鏃堕挓锛屽璁℃暟妯″紡锛岃鏁板櫒娓呯┖
    TB0CCR0 = 100 * 25;
    TB0CCTL2 = OUTMOD_7;
    TB0CCTL3 = OUTMOD_7; // 璁剧疆杈撳嚭妯″紡7
    TB0CCTL4 = OUTMOD_7;
    TB0CCTL5 = OUTMOD_7;
    TB0CCR2 = 0;
    TB0CCR3 = 0;
    TB0CCR4 = 0;
    TB0CCR5 = 0;

    P3DIR |= BIT6; // 閫夋嫨3.6寮曡剼杈撳嚭50Hzpwm
    P3SEL |= BIT6;
    TA2CTL |= TASSEL_1 + MC_1 + TACLR; // 閫夋嫨ACLK鏃堕挓锛屽璁℃暟妯″紡锛岃鏁板櫒娓呯┖
    TA2CCR0 = 656;
    TA2CCTL1 = OUTMOD_7;
    TA2CCR1 = 330;

    // 缂栫爜鍊艰鍙栨ā鍧�4涓彛
    P1DIR &= ~(BIT2 + BIT4); // 閫夋嫨1.2寮曡剼鎹曡幏绗竴涓笂鍗囨部锛�1.4寮曡剼鎹曡幏绗簩涓笂鍗囨部
    P1SEL |= BIT2 + BIT4;
    TA0CTL |= TASSEL_2;           // 閫夋嫨SMCLK鏃堕挓
    TA0CCTL1 = CAP + CM_1 + CCIE; // 鎹曡幏妯″紡锛屾崟鑾蜂笂鍗囨部
    TA0CCTL3 = CAP + CM_1 + CCIE;

    // 瀹氭椂鍣ㄦā鍧楋紝姣忛殧1s璁＄畻褰撳墠杞�燂紝1涓彛
    P3DIR |= BIT2;
    P3SEL |= BIT2;
    TA1CTL |= MC_1 + TASSEL_1 + TACLR; //鏃堕挓涓篈CLK锛岃繛缁鏁版ā寮忥紝姣旇緝妯″紡锛屽紑濮嬫椂娓呴浂璁℃暟鍣�
    TA1CCTL0 |= CCIE;                  //姣旇緝鍣ㄤ腑鏂娇鑳�
    TA1CCR0 = 16;                      //16 = 2kHZ
    TA1CCTL1 = OUTMOD_7;
    TA1CCR1 = 8;
}

void Set_move(int PWML, int PWMR)
{
    int temp;
    if (PWML < 0) // 宸﹁疆鍚戝悗杩愬姩
    {
        temp = -PWML * 25;
        TB0CCR2 = temp < pwm_min ? pwm_min : (temp > pwm_max ? pwm_max : temp);
        TB0CCR3 = 0;
    }
    else
    {
        temp = PWML * 25;
        TB0CCR3 = temp < pwm_min ? pwm_min : (temp > pwm_max ? pwm_max : temp);
        TB0CCR2 = 0;
    }
    if (PWMR < 0) // 鍙宠疆鍚戝悗杩愬姩
    {
        temp = -PWMR * 25;
        TB0CCR4 = temp < pwm_min ? pwm_min : (temp > pwm_max ? pwm_max : temp);
        TB0CCR5 = 0;
    }
    else
    {
        temp = PWMR * 25;
        TB0CCR5 = temp < pwm_min ? pwm_min : (temp > pwm_max ? pwm_max : temp);
        TB0CCR4 = 0;
    }
}

void Set_turn(int c)
{
    c = c < 0 ? c>>1 : c;
    c = c > 56 ? 56 : c < -28 ? -28 : c;
    TA2CCR1 = (462 + c * 82 / 15) / 10;
}

void Position_PID(int RealL, int TargetL, int RealR, int TargetR)
{
    static int BiasL, PwmL, Integral_biasL, Diff_biasL, Last_BiasL; // 宸﹁疆鍙傛暟
    static int BiasR, PwmR, Integral_biasR, Diff_biasR, Last_BiasR; // 鍙宠疆鍙傛暟

    BiasL = TargetL - RealL;
    Integral_biasL += BiasL;
    Diff_biasL = BiasL - Last_BiasL;
    PwmL = P_coeff * BiasL + Integral_biasL / I_coeff_div + D_coeff * Diff_biasL; // Pwm100 瀵瑰簲 3鍦�/s
    PwmL = PwmL / 60;
    Last_BiasL = BiasL;

    BiasR = TargetR - RealR;
    Integral_biasR += BiasR;
    Diff_biasR = BiasR - Last_BiasR;
    PwmR = P_coeff * BiasR + Integral_biasR / I_coeff_div + D_coeff * Diff_biasR;
    PwmR = PwmR / 60;
    Last_BiasR = BiasR;

    Set_move(PwmL, PwmR);
}

void track(long speed) // 浠ヤ竴瀹氶�熷害寰抗杩愬姩
{
    int alpha = 0;
    int rate = 0; // 100鍊嶉�熷害

    static int status = 0;

    switch (status)
    {
    case 0:
        if (NM)
        {
            alpha += 0;
            rate += 0;
        }
        else if (R4)
        {
            alpha += 56;
            rate += 40;
            status = 1;
        }
        else if (R3)
        {
            alpha += 16;
            rate += 8;
        }
        else if (R2)
        {
            alpha += 8;
            rate += 4;
        }
        else if (R1)
        {
            alpha += 4;
            rate += 2;
        }
        else if (L4)
        {
            alpha -= 56;
            rate -= 40;
            status = 2;
        }
        else if (L3)
        {
            alpha -= 16;
            rate -= 8;
        }
        else if (L2)
        {
            alpha -= 8;
            rate -= 4;
        }
        else if (L1)
        {
            alpha -= 4;
            rate -= 2;
        }
        break;
    case 1:
        if (L1 || L2 || L3 || L4)
        {
            alpha += 0;
            rate += 0;
            status = 0;
        }
        else
        {
            alpha += 56;
            rate += 40;
        }
        break;
    case 2:
        if (R1 || R2 || R3 || R4)
        {
            alpha += 0;
            rate += 0;
            status = 0;
        }
        else
        {
            alpha -= 56;
            rate -= 40;
        }
        break;
    }


    Set_turn(alpha);
    Tar_speedL = speed * (100 + rate) / 100;//(int)(speed * (1 + 0.54 * tan(alpha)));
    Tar_speedR = speed * (100 - rate) / 100;//(int)(speed * (1 - 0.54 * tan(alpha)));
    Position_PID(left_speed, Tar_speedL, right_speed, Tar_speedR);
}

// 鏍稿績鎺у埗妯″潡
#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer1_A0(void)
{
    static int timer_count = 0;

    static int count = 0;

    if (timer_count < 96)
    {
        timer_count++;
    }
    else
    {
        timer_count = 0;

        P8OUT ^= BIT1;

        if (mode == 2) // PREPARE
        {
            count = 1;
            realJourney = 0;
            tarJourney = 48000;
            realTime = 0;
            tarTime = time * 20;
            return;
        }

        if (mode == 3) // RUN
        {
            if (count == 1)
            {
                left_count = 0;
                right_count = 0;
                count = 0;
            }

            realTime++;

            if (ED) // 鍒拌揪缁堢偣
            {
                mode = 4;
                return;
            }

            // 閫熷害璁＄畻锛�0.05s杞殑鍦堟暟*10000锛�0.5鍦�/s -> 250锛岃疆瀛愯浆閫�*500锛�0.05m/s == 0.12鍦�/s == 鍙傛暟60
            left_speed = 1000 / 39 * left_count;
            right_speed = 1000 / 39 * right_count;

            realJourney += (left_count + right_count) * 1282 / 100; // 鍖栫畝鍓�/ 2 / 390 * 5 / 24 * 2400 * 20

            // 娓呴浂璁℃暟
            left_count = 0;
            right_count = 0;

            if (realTime >= tarTime || realJourney >= tarJourney) // 鏃堕棿鎴栬窛绂诲埌浜嗚繕娌＄绾�
            {
                track(tarJourney / tarTime);
                return;
            }

            track((tarJourney - realJourney) / (tarTime - realTime));

            return;
        }

        if (mode == 4) // PAUSE
        {
            Set_move(0, 0);
            return;
        }
    }
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void Timer0_A1(void)
{
    switch (TA0IV & 0x0E)
    {
    case 2: // 鍙宠疆涓婂崌娌�
        right_count++;
        break;
    case 6: // 宸﹁疆涓婂崌娌�
        left_count++;
        break;
    }
}
