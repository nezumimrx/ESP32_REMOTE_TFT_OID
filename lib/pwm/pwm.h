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
void basic_movements(int condition,int MotorSpeed);
void remote_control_behaviors(int button);
boolean pwm_oid_movement(int previous_face_to,int move_direction);//previous_face_to->1-朝Y轴负方向，2-朝Y轴正方向，3-朝X轴负方向，4-朝X轴正方向，0-无初始朝向（自由摆放）
boolean pwm_oid_reposition(int target_x,int target_y,int target_angle,int MotorSpeed);
#endif