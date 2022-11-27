#include <Preferences.h>
extern Preferences pref; //用来存用户自定义的音量到eeprom里
extern int volume;//音量
extern int Mode_Switch;//0-遥控模式, 1 -编程闯关模式，2-编程积分模式
extern TaskHandle_t TFT_TASK_Handle;
extern int face_condition;
extern boolean connected_with_controller;//espnow专用，持续发送（‘0’，0）判断是否连接了遥控


//OID parameters
extern int OID_X;
extern int OID_Y;
extern int OID_Angle;
extern boolean OID_available;
extern int OID_Index;
extern uint8_t OID_code_type;

