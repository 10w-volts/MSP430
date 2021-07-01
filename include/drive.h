/*
 * drive.h
 *
 *  Created on: 2020��10��10��
 *      Author: cjp
 */

#ifndef DRIVE_H_
#define DRIVE_H_

extern int woc;

extern int mode;
extern unsigned int time;
extern unsigned int angle;

extern unsigned long tarJourney;
extern int tarTime;
extern unsigned long realJourney;
extern int realTime;

extern int P_coeff;  // PID�����е�ϵ��
extern int I_coeff_div;
extern int D_coeff;

extern int pwm_min;
extern int pwm_max;

extern unsigned int Tar_speedL; // ����Ŀ���ٶ�
extern unsigned int Tar_speedR; // ����Ŀ���ٶ�

extern unsigned int left_speed; // �����ٶ�
extern unsigned int right_speed; // �����ٶ�

extern unsigned int left_count; // ���ֱ���ֵ
extern unsigned int right_count; // ���ֱ���ֵ

void init_drive(void);

void Set_move(int PWML, int PWMR);

void Set_turn(int c);

void Position_PID(int RealL, int TargetL, int RealR, int TargetR);

void track(long speed);

#endif /* DRIVE_H_ */
