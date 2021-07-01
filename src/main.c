#include <msp430.h>
#include <set_clock.h>
#include "user_io.h"
#include "drive.h"
#include "sensor.h"

void loop(void);

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;

    SetClock_MCLK12MHZ_SMCLK12MHZ_ACLK32_768K(); //25Mhz
    init_oled();
    init_key();
    init_drive();
    INIT_SENSOR;

    show();
    __enable_interrupt();
    while (1)
    {
//        if (get_key())
//        {
//            TA1CCTL1 = TA1CCTL1 == OUTMOD_0 ? OUTMOD_7 : OUTMOD_0;
//        }
        loop();
    }
}

void loop(void)
{
    switch (mode)
    {
    case 0: // CHOOSE_TIME:
        TA1CCTL1 = OUTMOD_0;
        if (get_key())
        {
            time == 20 ? time = 10 : time++;
        }
        show();
        break;
    case 1: // CHOOSE_ANGLE:
        if (get_key())
        {
            angle = angle == 40 ? 0 : angle == 0 ? 11 : angle == 30 ? 40 : angle + 1;
            pwm_max = angle == 0 ? 68 * 25 : angle <= 15 ? (75 + angle / 5 * 2) * 25 : (76 + angle / 5 * 2) * 25;
        }
        show();
        break;
    case 2: // PREPARE:
//        Set_turn(28*3);
        if (get_key())
        {
            if (woc==1)
            {
                woc = 0;
                P8OUT |= BIT0;
            }
            else
            {
                woc = 1;
                P8OUT &= ~BIT0
            }
        }
        show();
        break;
    case 3: // RUN:
        show();
        break;
    case 4: // PAUSE:
        TA1CCTL1 = OUTMOD_7;
        show();
        break;
    }
}
