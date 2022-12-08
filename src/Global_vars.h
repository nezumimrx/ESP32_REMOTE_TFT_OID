#include <Preferences.h>
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
extern int button_pressed;//123456前后左右平移13左前14右前23左后24右后37模式38清空39撤回40音量41运行
//
//Start-up Parameters
extern boolean robot_started;
//OID Parameters
extern int OID_X;
extern int OID_Y;
extern int OID_Angle;
extern boolean OID_available;
extern int OID_Index;
extern uint8_t OID_code_type;
//PWM Parameters
extern int Mode_0_Speed;
//Voice Parameters
extern int receive_voice_condition;
extern int Speaker_volume;


