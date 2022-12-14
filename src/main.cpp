#include <Arduino.h>
#include <pwm.h>
#include <RemoteReceive.h>
#include <OID_Card_Read.h>
#include <TFT.h>
#include <OID.h>
#include <Global_vars.h>
#include <Voice.h>
#include <Espnow_slave.h>
#include <SerialCmd.h>
#include <lvgl.h>
#include <CodeParse.h>


//*parameters 
//robot_started在build_connection_funcs结尾，确保在启动语音以及连接遥控语音播放完毕之后才可操控
boolean robot_started=false;
//mode_switch,操控的几种不同模式

int Mode_Switch=0;//0-遥控模式, 1 -编程闯关模式，2-编程积分模式
TaskHandle_t TFT_TASK_Handle;
TaskHandle_t OID_TASK_Handle;
TaskHandle_t Code_Process_Handle;
int face_condition=0;//0-默认脸，1-编程脸

int Speaker_volume = 15;//初始音量设置为15
boolean start_cypher=false; //是通过check_code，可以开始运行代码
boolean instant_stop=false;
String code_str_raw="&";
String code_str_clean="";
boolean on_coding_map=false;

int test_int=0;
//*Tasks

void OID_TASK(void*parameters){
  for(;;){
    OID_scan();//每1ms更新
    Check_on_coding_map();//看看是否在故事地图上，在DataCard.cpp中，最终确认on_coding_map的值
    Card_Data_Analyse();
    Mask_StartCell_StoryMode();
    vTaskDelay(1/portTICK_PERIOD_MS);
  }
}

void TICK_TASK(void*parameters){
  for(;;){
    lv_timer_handler(); /* let the GUI do its work */
    vTaskDelay(5/portTICK_PERIOD_MS);
  }
}

void Timer_TASK(void*parameters){
  for(;;){
    
    if(off_coding_map_test_start){
      off_coding_map_counter++;
      if(off_coding_map_counter>25){
        on_coding_map=false;//对应OID_scan()里，在这段时间连续统计过了，确定不在地图范围内
        on_Story_Stage_Level=0;//离开地图了这个on_Story_Stage_Level也要清0，恢复为没有读取到关卡状态，否则Mask_OID还有数据，
      }
    }
    vTaskDelay(10/portTICK_PERIOD_MS);
  }
}

void TFT_TASK(void*parameters){
  
  for(;;){
    if(face_condition==0){
      TFT_usualExpression();
    }
    vTaskDelay(10/portTICK_PERIOD_MS);
  }
}

void Code_Process_TASK(void*parameters){//遥控器命令处理
  
  for(;;){
    if(start_cypher==true){
      code_parse(code_str_clean);
      start_cypher=0;
    }
    vTaskDelay(15/portTICK_PERIOD_MS);
  }
}


void setup() {
  Serial.begin(9600);
  Serial.setTimeout(25);
  Serial.println("Sugar Eureka 20221117!");
  pwm_init();
  TFT_func_init();
  OID_Init();
  volume_read_memory();//从pref里面读取语音预设
  espnow_slave_init();
  xTaskCreate(OID_TASK,"OID_TASK",5000,NULL,1,&OID_TASK_Handle);
  xTaskCreate(TICK_TASK,"TICK_TASK",3000,NULL,3,NULL);
  xTaskCreate(Timer_TASK,"Timer_TASK",3000,NULL,4,NULL);
  xTaskCreate(TFT_TASK, "TFT_TASK", 40000, NULL, 2, &TFT_TASK_Handle);
  xTaskCreate(Code_Process_TASK, "Code_Process_TASK", 7000, NULL, 1, &Code_Process_Handle);
}

void loop() {
  SerialCommands();
  if(connected_with_controller==true){
    build_connection_funcs();
    Remote_Data_Analyse();//在循环中快速接收指令
    //卡片识别玩法，提示用户请将我放在带有方向指令的卡片上进行游戏
    
    
  }else if(connected_with_controller==false){
    lost_connection_funcs();
  }
  
  //pwm_oid_reposition(50,50,0,255);
}