#ifndef _pwm_H_
#define _pwm_H_
#include <Arduino.h>

void pwm_init();
void pwm_motion(int a1,int a2,int b1,int b2,int c1,int c2,int d1,int d2);
void pwm_forward(int MotorSpeed);
void pwm_backward(int MotorSpeed);
void pwm_rotate_left(int MotorSpeed);
void pwm_rotate_right(int MotorSpeed);
void mecanum_left(int MotorSpeed);
void mecanum_right(int MotorSpeed);
void pwm_stop();
void pwm_local_process_code_behaviors(int condition,int MotorSpeed);
void pwm_oid_reposition(int target_x,int target_y,int target_angle,int MotorSpeed);

#endif