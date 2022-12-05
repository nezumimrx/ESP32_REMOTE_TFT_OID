#include <Arduino.h>
#include <pwm.h>
#include <TFT.h>
#include <OID.h>
#include <Global_vars.h>
#include <Voice.h>
#include <Espnow_slave.h>
#include <SerialCmd.h>
#include <lvgl.h>
//*parameters 
//robot_started在build_connection_funcs结尾，确保在启动语音以及连接遥控语音播放完毕之后才可操控
boolean robot_started=false;
//mode_switch,操控的几种不同模式

int Mode_Switch=0;//0-遥控模式, 1 -编程闯关模式，2-编程积分模式
TaskHandle_t TFT_TASK_Handle;
TaskHandle_t OID_TASK_Handle;
int face_condition=0;//0-默认脸，1-编程脸

//*Tasks

void OID_TASK(void*parameters){
  for(;;){
    OID_scan();//每1ms更新
    vTaskDelay(1/portTICK_PERIOD_MS);
  }
}

void TICK_TASK(void*parameters){
  for(;;){
    lv_timer_handler(); /* let the GUI do its work */
    vTaskDelay(5/portTICK_PERIOD_MS);
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


void setup() {
  Serial.begin(115200);
  Serial.setTimeout(25);
  Serial.println("Sugar Eureka 20221117!");
  pwm_init();
  TFT_func_init();
  OID_Init();
  volume_read_memory();//从pref里面读取语音预设
  espnow_slave_init();
  xTaskCreate(OID_TASK,"OID_TASK",3000,NULL,1,&OID_TASK_Handle);
  xTaskCreate(TICK_TASK,"TICK_TASK",3000,NULL,3,NULL);
  xTaskCreate(TFT_TASK, "TFT_TASK", 40000, NULL, 2, &TFT_TASK_Handle);
}

void loop() {
  SerialCommands();
  if(connected_with_controller==true){
    build_connection_funcs();
    //卡片识别玩法，提示用户请将我放在带有方向指令的卡片上进行游戏
  }else if(connected_with_controller==false){
    lost_connection_funcs();
  }
  //pwm_oid_reposition(50,50,0,255);
}