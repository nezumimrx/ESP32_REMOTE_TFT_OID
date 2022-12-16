#include <Arduino.h>
#include <Espnow_slave.h>
#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <Voice.h>
#include <Global_vars.h>
#include <RemoteReceive.h>

boolean connected_with_controller = false; //连没连上控制器
boolean first_time_lost_connection=true;//首次运行若未连接控制器则播放未连接语音
boolean first_time_build_connection=true;
esp_now_peer_info_t peerInfo;

typedef struct data_to_send
{
  int x;
  int y;
} data_to_send;
data_to_send send_data;

typedef struct data_to_recv
{
    char x;
    int y;
} data_to_recv;
data_to_recv received_data;

void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t sendStatus)
{
    if (sendStatus == 0){connected_with_controller=true;}//Serial.println("send sucess!");
    else{connected_with_controller=false;}//Serial.println("fail");
}

void onDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len){
    memcpy(&received_data, incomingData, sizeof(received_data));
    DataProcess(received_data.x,received_data.y);
    received_data.x='0';
    received_data.y=0;
    // Serial.print("Bytes received: ");
    // Serial.println(len);
    // Serial.print("x: ");
    // Serial.println(received_data.x);
    // Serial.print("y: ");
    // Serial.println(received_data.y);
    // Serial.println();
    delay(5);
}

void espnow_slave_init()
{
  // change MAC address
  WiFi.mode(WIFI_STA);
  esp_wifi_set_mac(WIFI_IF_STA,&myaddress[0]);
  Serial.print("Current my add: ");Serial.println(WiFi.macAddress());
  WiFi.disconnect();
  //
  if(esp_now_init()!=0){
    Serial.println("Communication failed to init!");
    return;
  }
  esp_now_register_send_cb(onDataSent);
  memcpy(peerInfo.peer_addr, masteraddress, 6);
  peerInfo.channel =0;
  peerInfo.encrypt = false;
  if(esp_now_add_peer(&peerInfo)!=ESP_OK){
    Serial.println("Communication to peer failed!");
  }
  esp_now_register_recv_cb(onDataRecv);
}

void send_data_now(char c,int num){
  send_data.x = c;
  send_data.y = num;
  esp_now_send(masteraddress, (uint8_t *)&send_data, sizeof(send_data));
}

void lost_connection_funcs(){
    send_data_now('0',0);//检测是否连接了控制器
    
    if(first_time_lost_connection){
      //播放丢失连接语音
      int random_play_num=random(90,92);
      play_voice(random_play_num);
      first_time_lost_connection=false;
      first_time_build_connection=true;
      //还原配置

      //需要重新播放连接成功语音
      robot_started=false;
    }
}

void build_connection_funcs(){
    if(first_time_build_connection){
      //播放建立连接语音
      int random_play_num=random(92,94);
      play_voice(random_play_num);
      first_time_build_connection=false;
      first_time_lost_connection=true;
      //初始化所有功能

      //启动完成，可以进行遥控
      robot_started=true;

  }
}
