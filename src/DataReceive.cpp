#include <Arduino.h>
#include <Global_vars.h>
boolean receive_data_flag=false;//是否接受到信号
boolean remote_running=false;//是否在运行动作
int button_pressed=99;//123456前后左右平移7模式8清空9撤回10音量11运行
void DataProcess(char dataX, int dataY){
    button_pressed=99;
    if(robot_started){
        if(dataX=='W'&&dataY==0){
            button_pressed=0;
        }else if(dataX=='W'&&dataY==1){//前
            button_pressed=1;
        }else if(dataX=='W'&&dataY==2){//后
            button_pressed=2;
        }else if(dataX=='W'&&dataY==3){//左
            button_pressed=3;
        }else if(dataX=='W'&&dataY==4){//右
            button_pressed=4;
        }else if(dataX=='W'&&dataY==5){//左平
            button_pressed=5;
        }else if(dataX=='W'&&dataY==6){//右平
            button_pressed=6;
        }else if(dataX=='M'){
            button_pressed=7;
        }else if(dataX=='F'&&dataY==20){//清空指令按键
            button_pressed=8;
        }else if(dataX=='F'&&dataY==19){//删除指令按键
            button_pressed=9;
        }
        else if(dataX=='V'){//音量控制按键
            button_pressed=10;
        }else if(dataX=='R'){//运行按键
            button_pressed=11;
        }
        
        
        
        else if(dataX=='s'){//速度控制
            
        }else if(dataX=='0'){
            connected_with_controller=true;
        }
    }
}

void Mode_0_Funcs(char dataX, int dataY){
    receive_data_flag=true;
    if(dataX=='W'){
        if(dataY==0){receive_wheel_condition=0;remote_running=false;}
        else if(dataY==1){receive_wheel_condition=1;remote_running=true;receive_voice_condition=14;}
        else if(dataY==2){receive_wheel_condition=2;remote_running=true;receive_voice_condition=14;}
        else if(dataY==3){receive_wheel_condition=3;remote_running=true;receive_voice_condition=14;}
        else if(dataY==4){receive_wheel_condition=4;remote_running=true;receive_voice_condition=14;}
        else if(dataY==5){receive_wheel_condition=5;remote_running=true;receive_voice_condition=14;}
        else if(dataY==6){receive_wheel_condition=6;remote_running=true;receive_voice_condition=14;}
    }
}