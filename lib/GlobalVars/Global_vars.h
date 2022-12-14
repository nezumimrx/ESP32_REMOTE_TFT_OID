#include <Preferences.h>
#define Mode_0_Speed 255//遥控模式的电机转速
#define Mode_1_Speed 255//编程模式的电机转速

#define Max_Play_Modes 3 //一共有几种玩法
extern Preferences pref; //用来存用户自定义的音量到eeprom里
//游戏的几种模式
extern int Mode_Switch;//0-遥控模式, 1 -编程闯关模式，2-编程积分模式
//是否收到了有用的信息
extern boolean receive_data_flag;
//是否小车正因为接收到remote信号在动
extern boolean remote_running;
//TFT相关全局变量
extern TaskHandle_t TFT_TASK_Handle;
extern int face_condition;
//espnow专用，是否连接了遥控器
extern boolean connected_with_controller;//espnow专用，持续发送（‘0’，0）判断是否连接了遥控
//接收到遥控器按下了哪个按键，单纯为了代码方便把数字加到37往后
extern int button_pressed;//123456前后左右平移13左前14右前23左后24右后/25循环2次/26循环3次/27循环结束/28条件1开始/29条件1结束/30条件2开始/31条件2结束/32条件3开始/33条件3结束/34交互1/35交互2/36交互3/37模式38清空39撤回40音量41运行
                          //其中button_pressed变为25-36之间的数字并不是由遥控器输入的，而是由用户读取卡片从而实现的，例如用户录入指令阶段，读取了一张场景交互卡1，则同样会触发button_pressed=34
//
//Start-up Parameters
extern boolean robot_started;
//OID Parameters
extern int OID_X;//真实的OID读取到的坐标
extern int OID_Y;//真实的OID读取到的坐标

extern int OID_Angle;
extern boolean OID_available;
extern int OID_Index;
extern uint8_t OID_code_type;
//PWM Parameters


//Voice Parameters
extern int play_voice_condition;
extern int Speaker_volume;
extern boolean need_play_voice;
//Coding Parameters
extern TaskHandle_t Code_Process_Handle;//在线程中处理编程指令的代码
extern uint8_t able_to_cypher;//是否满足解析条件,0-满足 1-第一种不满足条件，2-第二种不满足条件....
extern boolean start_cypher;//是否正在执行编程指令
extern boolean instant_stop;//紧急停止
extern String code_str_raw;//录入的原始指令
extern String code_str_clean;

extern boolean has_condition_type1;//在存储code_str_condition_type1的同时，将has_condition_type1置为1，没有则为0
extern boolean receive_condition_type1;//是否踩到了对应的卡片上
extern String code_str_condition_type1;//存储type1符号表示的条件语句，type1 目前为{}中的语句

extern boolean has_condition_type2;
extern boolean receive_condition_type2;//是否踩到了对应的卡片上
extern String code_str_condition_type2;//存储type2符号表示的条件语句, type2 目前为[]中的语句

extern boolean has_condition_type3;
extern boolean receive_condition_type3;//是否踩到了对应的卡片上
extern String code_str_condition_type3;//存储type3符号表示的条件语句, type3 目前为||中的语句

#define commands_size 50
extern int commands_array[commands_size];
extern int commands_counter;

//故事地图编程Map Parameters这里要改成最后的故事地图尺寸
#define map_start_X 0
#define map_start_Y 0
#define map_end_X 143
#define map_end_Y 207
#define oid_cell_unit_size 57 //OID坐标每+1，对应长度增加1.4，暂定一个格子是79.8mm(80mm)也就是57个单位坐标
// #define Story_StartPoint_X_start 5000
// #define Story_StartPoint_X_end 5057
// #define Story_StartPoint_Y_start 500

#define Story_StartPoint_X_start 0 //故事模式开始格子命名的坐标，不是真实坐标，例如OID会读到5000+的数据，但是在地图上的位置却是50之类的，是规定的数据
#define Story_StartPoint_Y_start 0
#define Total_Story_Level 30 //一共多少关
#define Story_StartPoint_X_end (Story_StartPoint_X_start+oid_cell_unit_size)//故事模式开始格子结束坐标
#define Story_StartPoint_Y_end ((Total_Story_Level+1)*oid_cell_unit_size)  //(30+1)*1 先设30关
#define Story_Level_1_X 30 //故事模式开始格子的真实坐标，也就是在地图上，这个格子的连贯坐标,Mask_OID_X
#define Story_Level_1_Y 50 //故事模式开始格子的真实坐标，也就是在地图上，这个格子的连贯坐标,Mask_OID_Y
#define Story_Level_2_X 30 //故事模式开始格子的真实坐标，也就是在地图上，这个格子的连贯坐标,Mask_OID_X
#define Story_Level_2_Y 50 //故事模式开始格子的真实坐标，也就是在地图上，这个格子的连贯坐标,Mask_OID_Y
#define Story_Level_3_X 30 //故事模式开始格子的真实坐标，也就是在地图上，这个格子的连贯坐标,Mask_OID_X
#define Story_Level_3_Y 50 //故事模式开始格子的真实坐标，也就是在地图上，这个格子的连贯坐标,Mask_OID_Y
#define Story_Level_4_X 30 //故事模式开始格子的真实坐标，也就是在地图上，这个格子的连贯坐标,Mask_OID_X
#define Story_Level_4_Y 50 //故事模式开始格子的真实坐标，也就是在地图上，这个格子的连贯坐标,Mask_OID_Y

extern int Masked_OID_X;//由于在故事模式，有些格子会被替换，所以在执行pwm_calculate_movement的时候代入的是MaskedOIDX
extern int Masked_OID_Y;
extern int my_face_direction;//当前小车在地图上的朝向，1-朝上(y负方向)2朝下3朝左4朝右，每个关卡开始时，需要设定这个变量,0-以当前位置为起始点和方向进行调整
                             //这个变量主要会带入pwm_oid_movement(int previous_face_to,int move_direction)中，用于确定上一次移动之后的朝向，在code_parse的oid_wheel()里，每次移动之后都会更新my_face_direction,小车被拿起或切换到其他模式时自动重置
extern boolean on_coding_map;//踩在故事地图上
extern int on_Story_Stage_Level;//0-没踩到任何故事地图的起始点 1,2,3----对应关卡依次类推
extern boolean process_based_on_OID_coordinates;//在check_code()时，决定是否根据地图来移动，如果根据地图来移动则为true，否则根据延迟来移动为false
extern int off_coding_map_counter;//如果在这段时间内持续检测到离开了
extern boolean off_coding_map_test_start;//检测离开地图，如果这段时间一直OID_available都是false，那么就是真的离开地图了


