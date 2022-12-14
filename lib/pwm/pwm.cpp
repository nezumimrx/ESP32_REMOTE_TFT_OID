#include <Arduino.h>
#include <Global_vars.h>
#include <pwm.h>

/*PWM使用的是ledc模块功能实现的，一共有16个通道和4个Timer
 * LEDC Chan to Group/Channel/Timer Mapping
 ** ledc: 0  => Group: 0, Channel: 0, Timer: 0
 ** ledc: 1  => Group: 0, Channel: 1, Timer: 0
 ** ledc: 2  => Group: 0, Channel: 2, Timer: 1
 ** ledc: 3  => Group: 0, Channel: 3, Timer: 1
 ** ledc: 4  => Group: 0, Channel: 4, Timer: 2
 ** ledc: 5  => Group: 0, Channel: 5, Timer: 2
 ** ledc: 6  => Group: 0, Channel: 6, Timer: 3
 ** ledc: 7  => Group: 0, Channel: 7, Timer: 3
 ** ledc: 8  => Group: 1, Channel: 0, Timer: 0
 ** ledc: 9  => Group: 1, Channel: 1, Timer: 0
 ** ledc: 10 => Group: 1, Channel: 2, Timer: 1
 ** ledc: 11 => Group: 1, Channel: 3, Timer: 1
 ** ledc: 12 => Group: 1, Channel: 4, Timer: 2
 ** ledc: 13 => Group: 1, Channel: 5, Timer: 2
 ** ledc: 14 => Group: 1, Channel: 6, Timer: 3
 ** ledc: 15 => Group: 1, Channel: 7, Timer: 3
*/
boolean DEBUG=0;//这个打开会输出详细的移动数据
const int pwmA1 = 26;
const int pwmA2 = 27;
const int pwmB1 = 32;
const int pwmB2 = 33;
const int pwmC1 = 22;
const int pwmC2 = 21;
const int pwmD1 = 16;
const int pwmD2 = 17;

const int pwmA1_channel = 2;
const int pwmA2_channel = 3;
const int pwmB1_channel = 10;
const int pwmB2_channel = 11;
const int pwmC1_channel = 4;
const int pwmC2_channel = 5;
const int pwmD1_channel = 12;
const int pwmD2_channel = 13;

const int pwm_freq = 1000; // 1000hz
// PWM分辨率，取值为 0-20 之间，这里填写为8，对应后面ledcWrite填写的值
// 这个里面填写的pwm值就在 0 - 2的8次方 之间 也就是 0-255
uint8_t pwm_res = 8;//PWM精度，2的8次方，8就是0-255如果是10就是0-1024
int angle_tolerance=10;//reposition的时候角度容差，和目标夹角范围小于10度就行了
int distance_tolerance=20;//reposition的时候距离容差范围，距离目标定位2cm就行了

void pwm_init()
{
    pinMode(pwmA1, OUTPUT);
    pinMode(pwmA2, OUTPUT);
    pinMode(pwmB1, OUTPUT);
    pinMode(pwmB2, OUTPUT);
    pinMode(pwmC1, OUTPUT);
    pinMode(pwmC2, OUTPUT);
    pinMode(pwmD1, OUTPUT);
    pinMode(pwmD2, OUTPUT);
    //Timer0是系统的，尽量不占用
    ledcSetup(pwmA1_channel, pwm_freq, pwm_res); //从上表可以看出通道2和通道3都用的是timer1，为了避开timer0，因此不选通道0和通道1
    ledcSetup(pwmA2_channel, pwm_freq, pwm_res); //从上表可以看出通道2和通道3都用的是timer1，为了避开timer0，因此不选通道0和通道1
    ledcSetup(pwmB1_channel, pwm_freq, pwm_res); //所有A\B口都用timer1产生
    ledcSetup(pwmB2_channel, pwm_freq, pwm_res); //所有A\B口都用timer1产生
    ledcSetup(pwmC1_channel, pwm_freq, pwm_res); // C\D口选的是timer2
    ledcSetup(pwmC2_channel, pwm_freq, pwm_res); // C\D口选的是timer2
    ledcSetup(pwmD1_channel, pwm_freq, pwm_res); // C\D口选的是timer2
    ledcSetup(pwmD2_channel, pwm_freq, pwm_res); // C\D口选的是timer2
    ledcAttachPin(pwmA1, 2);
    ledcAttachPin(pwmA2, 3);
    ledcAttachPin(pwmB1, 10);
    ledcAttachPin(pwmB2, 11);
    ledcAttachPin(pwmC1, 4);
    ledcAttachPin(pwmC2, 5);
    ledcAttachPin(pwmD1, 12);
    ledcAttachPin(pwmD2, 13);
}

void pwm_motion(int a1, int a2, int b1, int b2, int c1, int c2, int d1, int d2) // a1a2-左前，b1b2-右前，c1c2-左后，d1d2-右后
{
    ledcWrite(pwmA1_channel, a1);
    ledcWrite(pwmA2_channel, a2);
    ledcWrite(pwmB1_channel, b1);
    ledcWrite(pwmB2_channel, b2);
    ledcWrite(pwmC1_channel, c1);
    ledcWrite(pwmC2_channel, c2);
    ledcWrite(pwmD1_channel, d1);
    ledcWrite(pwmD2_channel, d2);
}

void pwm_forward(int MotorSpeed)
{
    pwm_motion(MotorSpeed, 0, MotorSpeed, 0, MotorSpeed, 0, MotorSpeed, 0);
    if(DEBUG)Serial.println("moving forward");
}

void pwm_backward(int MotorSpeed)
{
    pwm_motion(0, MotorSpeed, 0, MotorSpeed, 0, MotorSpeed, 0, MotorSpeed);
    if(DEBUG)Serial.println("moving backward");
}

void pwm_rotate_left(int MotorSpeed)
{
    pwm_motion(0, MotorSpeed, MotorSpeed, 0, 0, MotorSpeed, MotorSpeed, 0);
    if(DEBUG)Serial.println("turning left");
}

void pwm_rotate_right(int MotorSpeed)
{
    pwm_motion(MotorSpeed, 0, 0, MotorSpeed, MotorSpeed, 0, 0, MotorSpeed);
    if(DEBUG)Serial.println("turning right");
}

void mecanum_left(int MotorSpeed)
{
    pwm_motion(0, MotorSpeed, MotorSpeed, 0, MotorSpeed, 0, 0, MotorSpeed);
    if(DEBUG)Serial.println("mec move left");
}

void mecanum_right(int MotorSpeed)
{
    pwm_motion(MotorSpeed, 0, 0, MotorSpeed, 0, MotorSpeed, MotorSpeed, 0);
    if(DEBUG)Serial.println("mec move right");
}

void diagonal_forward_left(int MotorSpeed){
    pwm_motion(0, 0, MotorSpeed, 0, MotorSpeed, 0, 0, 0);
    if(DEBUG)Serial.println("diagonal_forward_left");
}

void diagonal_forward_right(int MotorSpeed){
    pwm_motion(MotorSpeed, 0, 0, 0, 0, 0, MotorSpeed, 0);
    if(DEBUG)Serial.println("diagonal_forward_right");
}
void diagonal_backward_left(int MotorSpeed){
    pwm_motion(0, MotorSpeed, 0, 0, 0, 0, 0, MotorSpeed);
    if(DEBUG)Serial.println("diagonal_backward_left");
}

void diagonal_backward_right(int MotorSpeed){
    pwm_motion(0, 0, 0, MotorSpeed, 0, MotorSpeed, 0, 0);
    if(DEBUG)Serial.println("diagonal_backward_right");
}

void pwm_stop()
{
    pwm_motion(0, 0, 0, 0, 0, 0, 0, 0);
    Serial.println("stop");
}

void pwm_local_process_code_behaviors(int condition,int MotorSpeed)
{
    if (condition == 0)pwm_stop();
    else if (condition == 1)pwm_forward(MotorSpeed);
    else if (condition == 2)pwm_backward(MotorSpeed);
    else if (condition == 3)pwm_rotate_left(MotorSpeed);
    else if (condition == 4)pwm_rotate_right(MotorSpeed);
    else if (condition == 5)mecanum_left(MotorSpeed);
    else if (condition == 6)mecanum_right(MotorSpeed);
}
//坐标系y轴朝下 需要通过滤波后的OID数据 currentX currentY currentAngle 以及目标坐标 targetX targetY 分析出自己的动作
//若需要到达指定位置后再调整朝向,则输入要调整的inposition_target_angle,如果不需要调整，inposition_target_angle处为0
boolean pwm_calculate_movement(int target_x,int target_y,int current_x,int current_y,int current_angle, int inposition_target_angle, int MotorSpeed){
    if(target_x!=current_x||target_y!=current_y){
        float delta_X =(target_x-current_x);
        float delta_Y =(target_y-current_y);
        float distance = sqrt(sq(delta_X)+sq(delta_Y));
        int target_Angle = 0;
        if(delta_X<0&&delta_Y<0){//当前定位在目标定位的右下
            target_Angle=degrees(atan(abs(float(delta_Y/delta_X))))+270; 
        }else if(delta_X>0&&delta_Y>0){//当前定位在目标定位的左上
            target_Angle=degrees(atan(abs(float(delta_Y/delta_X))))+90; 
        }else if(delta_X>0&&delta_Y<0){//当前定位在目标定位的左下
            target_Angle=degrees(atan(abs(float(delta_X/delta_Y)))); 
        }else if(delta_X<0&&delta_Y>0){//当前定位在目标定位的右上
            target_Angle=degrees(atan(abs(float(delta_X/delta_Y))))+180; 
        }else if(delta_X==0&&delta_Y>0){//当前定位在目标定位正上方
            target_Angle=180;
        }else if(delta_X==0&&delta_Y<0){//当前定位在目标定位正上方
            target_Angle=0;
        }else if(delta_X<0&&delta_Y==0){//当前定位在目标定位右侧
            target_Angle=270;
        }else if(delta_X>0&&delta_Y==0){//当前定位在目标定位左侧
            target_Angle=90;
        }
        if(DEBUG)Serial.print("Should turn to Angle: ");Serial.print(target_Angle);
        if(DEBUG)Serial.print(" Distance: ");Serial.println(distance);
        if(distance>=distance_tolerance){
            int delta_Angle=(current_angle-target_Angle);
            if((current_angle>=355&&current_angle<=360&&target_Angle<=5&&target_Angle>=0)||(target_Angle>=355&&target_Angle<=360&&current_angle<=5&&current_angle>=0)){
                if(DEBUG)Serial.println("around 360/0 degree move forward");
            }else if(abs(delta_Angle)>=angle_tolerance){
                if(delta_Angle<0&&abs(delta_Angle)>=180){
                    if(DEBUG)Serial.println("rotate left");
                    pwm_rotate_left(MotorSpeed);
                    return false;
                }else if(delta_Angle<0&&abs(delta_Angle)<180){
                    if(DEBUG)Serial.println("rotate right");
                    pwm_rotate_right(MotorSpeed);
                    return false;
                }else if(delta_Angle>0&&abs(delta_Angle)>=180){
                    if(DEBUG)Serial.println("rotate right");
                    pwm_rotate_right(MotorSpeed);
                    return false;
                }else if(delta_Angle>0&&abs(delta_Angle)<180){
                    if(DEBUG)Serial.println("rotate left");
                    pwm_rotate_left(MotorSpeed);
                    return false;
                }
            }else if(abs(delta_Angle)<angle_tolerance){
                if(DEBUG)Serial.println("just go forward");
                pwm_forward(MotorSpeed);
                return false;
            }
        }else if(distance<=distance_tolerance){
            int delta_Angle = (current_angle-inposition_target_angle);
            if((current_angle>=355&&current_angle<=360&&inposition_target_angle<=5&&inposition_target_angle>=0)||(inposition_target_angle>=355&&inposition_target_angle<=360&&current_angle<=5&&current_angle>=0)){
                if(DEBUG)Serial.println("around 360/0 degree stop rotation,In position and angle");
                pwm_stop();
                return true;
            }else {
                if(DEBUG)Serial.print("delta Angle: ");Serial.println(delta_Angle);
                if(abs(delta_Angle)>=angle_tolerance){
                    if(delta_Angle<0&&abs(delta_Angle)>=180){
                        if(DEBUG)Serial.println("rotate left");
                        pwm_rotate_left(MotorSpeed);
                        return false;
                    }else if(delta_Angle<0&&abs(delta_Angle)<180){
                        if(DEBUG)Serial.println("rotate right");
                        pwm_rotate_right(MotorSpeed);
                        return false;
                    }else if(delta_Angle>0&&abs(delta_Angle)>=180){
                        if(DEBUG)Serial.println("rotate right");
                        pwm_rotate_right(MotorSpeed);
                        return false;
                    }else if(delta_Angle>0&&abs(delta_Angle)<180){
                        if(DEBUG)Serial.println("rotate left");
                        pwm_rotate_left(MotorSpeed);
                        return false;
                    }
                }else if(abs(delta_Angle)<angle_tolerance){
                    if(DEBUG)Serial.println("In position and angle");
                    pwm_stop();
                    return true;
                }
            }
        }             
    }
}

boolean pwm_oid_reposition(int target_x,int target_y,int target_angle,int MotorSpeed){
    //线程使用while内循环，保证小车到达位置之后不再循环
    boolean in_position=false;
    if(target_x<=0)target_x=0;
    if(target_y<=0)target_y=0;
    while(in_position==false){
        //Serial.print("X: ");Serial.print(OID_X);Serial.print(" Y: ");Serial.print(OID_Y);Serial.print(" Angle: ");Serial.println(OID_Angle);
        if(on_coding_map){//是否在地图上或者是否在关卡开始格子上
            in_position=pwm_calculate_movement(target_x,target_y,OID_X,OID_Y,OID_Angle,target_angle,MotorSpeed);
        }else {
            pwm_stop();
            Serial.println("off the map,need to restart game!");
            break;
        }
    }
    return in_position;//这里如果在while循环的时候一切顺利，则会从false变为true 证明已经移动到位置了
    
}
//previous_face_to 1-朝上，2朝下，3朝左，4朝右 move_direction 1-前进 2右转 3左转
boolean pwm_oid_movement(int previous_face_to,int move_direction){
    //previous_face_to->1-朝Y轴负方向，2-朝Y轴正方向，3-朝X轴负方向，4-朝X轴正方向，0-无初始朝向（自由摆放）
    //move_direction-> 1.向前移动1个单位格 2.向右旋转90度 3.向左旋转90度 
    //OID坐标每+1，对应长度增加1.4，暂定一个格子是79.8mm也就是57个单位坐标
    //preivous_direction如果不是0，那么就说明之前移动过一次，也就是有了一个方向，如果是0，说明是第一次走
    //move_direction是当前的移动方向
    int current_x=OID_X;//存下来，因为OID会变
    int current_y=OID_Y;
    int current_angle=OID_Angle;
    boolean finish_movement=false;//如果移动完了，会变为true
    //Serial.print("on_coding_map: ");Serial.println(on_coding_map);
    if(on_coding_map==true){
        if(previous_face_to==0){//之前没有前进过，是第一次在地图上走
            uint8_t face_to =0;//1-朝上，2-朝下，3-朝左，4-朝右
            int current_face_angle =0;//当前OID_Angle四舍五入朝向，只有0，90，180，270四个方向
            if((current_angle<=45&&current_angle>=0)||(current_angle<=359&&current_angle>315))
            {//绝对坐标朝前
                current_face_angle=0;
            }else if(current_angle>45&&current_angle<=135)
            {//绝对坐标朝右
                current_face_angle=90;
            }else if(current_angle>135&&current_angle<=225)
            {//绝对坐标朝下
                current_face_angle=180;
            }else if(current_angle>225&&current_angle<=315)
            {//绝对坐标朝左
                current_face_angle=270;
            }

            if(move_direction==2){//右转+90度
                finish_movement=pwm_oid_reposition(current_x,current_y,current_face_angle+90,Mode_1_Speed); 
            }else if(move_direction==3){//左转-90度
                if(current_face_angle==0)finish_movement=pwm_oid_reposition(current_x,current_y,270,Mode_1_Speed);
                else{finish_movement=pwm_oid_reposition(current_x,current_y,current_face_angle-90,Mode_1_Speed);}
            }else if(move_direction==1){//
                if(current_face_angle==0)finish_movement=pwm_oid_reposition(current_x,(current_y-oid_cell_unit_size),current_face_angle,Mode_1_Speed);
                else if(current_face_angle==90)finish_movement=pwm_oid_reposition((current_x+oid_cell_unit_size),current_y,current_face_angle,Mode_1_Speed);
                else if(current_face_angle==180)finish_movement=pwm_oid_reposition(current_x,(current_y+oid_cell_unit_size),current_face_angle,Mode_1_Speed);
                else if(current_face_angle==270)finish_movement=pwm_oid_reposition((current_x-oid_cell_unit_size),current_y,current_face_angle,Mode_1_Speed);
            }
            return finish_movement;
        }else if(previous_face_to!=0){//之前已经有个朝向，就不根据当前朝向再分析curren_face_angle了，直接依据之前的朝向调整位置
            int current_face_angle=0;
            if(previous_face_to==1){//朝上
                current_face_angle=0;
            }else if(previous_face_to==2){//朝下
                current_face_angle=180;
            }else if(previous_face_to==3){//朝左
                current_face_angle=270;
            }else if(previous_face_to==4){//朝右
                current_face_angle=90;
            }
            if(move_direction==2){//右转+90度
                finish_movement=pwm_oid_reposition(current_x,current_y,current_face_angle+90,Mode_1_Speed); 
            }else if(move_direction==3){//左转-90度
                finish_movement=pwm_oid_reposition(current_x,current_y,current_face_angle-90,Mode_1_Speed);
            }else if(move_direction==1){//
                if(current_face_angle==0)finish_movement=pwm_oid_reposition(current_x,(current_y-oid_cell_unit_size),current_face_angle,Mode_1_Speed);
                else if(current_face_angle==90)finish_movement=pwm_oid_reposition((current_x+oid_cell_unit_size),current_y,current_face_angle,Mode_1_Speed);
                else if(current_face_angle==180)finish_movement=pwm_oid_reposition(current_x,(current_y+oid_cell_unit_size),current_face_angle,Mode_1_Speed);
                else if(current_face_angle==270)finish_movement=pwm_oid_reposition((current_x-oid_cell_unit_size),current_y,current_face_angle,Mode_1_Speed);
            }
            return finish_movement;
        }
    }else {
        return false;
        Serial.println("off the map,need to restart game!");
    } 
}

void basic_movements(int condition,int MotorSpeed){
    if(condition==0){
        pwm_stop(); 
    }else if(condition==1){
        pwm_forward(MotorSpeed);
    }else if(condition==2){
        pwm_backward(MotorSpeed);
    }else if(condition==3){
        pwm_rotate_left(MotorSpeed);
    }else if(condition==4){
        pwm_rotate_right(MotorSpeed);
    }else if(condition==5){
        mecanum_left(MotorSpeed);
    }else if(condition==6){
        mecanum_right(MotorSpeed);
    }else if(condition==13){
        diagonal_forward_left(MotorSpeed);
    }else if(condition==14){
        diagonal_forward_right(MotorSpeed);
    }else if(condition==23){
        diagonal_backward_left(MotorSpeed);
    }else if(condition==24){
        diagonal_backward_right(MotorSpeed);
    }
}


void remote_control_behaviors(int button){//应包含基本的遥控功能及场景卡片识别功能（陷阱卡、buff卡、道具卡、声音卡）

    if(receive_data_flag==true){
        //Serial.print("Button before process: ");Serial.println(button_pressed);
        
        basic_movements(button,Mode_0_Speed);//基础遥控功能，前后左右平移斜移
        //还没实现卡片识别功能
        //step_on_card_behaviours();


        receive_data_flag=false;
    }
}












