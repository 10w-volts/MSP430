/*
 * sensor.h
 *
 *  Created on: 2020��10��11��
 *      Author: gxk
 */

#ifndef SENSOR_H_
#define SENSOR_H_

#define S0 (P9IN & BIT6)
#define S1 (P8IN & BIT5)
#define S2 (P4IN & BIT6)
#define S3 ((P8IN & BIT6)&&woc)
#define S4 (P5IN & BIT4)
#define S5 (P5IN & BIT3)
#define S6 (P5IN & BIT2)
#define S7 (P7IN & BIT6)
#define S8 (P9IN & BIT4)
#define S9 (P5IN & BIT5)

#define INIT_SENSOR (P4DIR &= ~BIT6, P5DIR &= ~(BIT2 + BIT3 + BIT4 + BIT5), P7DIR &= ~BIT6, P8DIR &= ~(BIT5 + BIT6), P9DIR &= ~(BIT4 + BIT6))

//  0 1 2 3 4
//  5 6 7 8 9
//
//  5+4+3

#define AW (!(S1 || S2 || S3 || S4 || S5 || S6 || S7 || S8 || S9))

#define NM ((S2 || S7) && !(S1 || S6 || S3 || S8) || (S1 || S6) && (S2 || S7) && (S3 || S8))

#define R4 ((S4 || S9) && !(S3 || S8))

#define R3 ((S3 || S8) && (S4 || S9) && !(S2 || S7))

#define R2 ((S3 || S8) && !(S2 || S7 || S4 || S9) || (S2 || S7) && (S3 || S8) && (S4 || S9))

#define R1 ((S2 || S7) && (S3 || S8) && !(S1 || S6 || S4 || S9))

#define L4 ((S0 || S5) && !(S1 || S6))

#define L3 ((S0 || S5) && (S1 || S6) && !(S2 || S7))

#define L2 ((S1 || S6) && !(S0 || S5 || S2 || S7) || (S0 || S5) && (S1 || S6) && (S2 || S7))

#define L1 ((S1 || S6) && (S2 || S7) && !(S0 || S5 || S3 || S8))

#define ED (S1 && S2 && S3 && (S0 || S4))

//// All White
//#define AW (!(S2 || S3 || S4 || S5))
//
//// 25��34�� || 2345ȫ��
//#define NM (S3 && S4 && !S2 && !S5 || !(S2 || S3 || S4 || S5))
//
//// 5��
//#define R3 (!!S5)
//
//// 45��
//#define R2 (S4 && S5)
//
//// 3��4��
//#define R1 (!S3 && S4)
//
//// 2��
//#define L3 (!!S2)
//
//// 23��
//#define L2 (S2 && S3)
//
//// 4��3��
//#define L1 (!S4 && S3)
//
//#define CT (S6 && S7)
//
//#define ED (S0 && S1)

#endif /* SENSOR_H_ */
