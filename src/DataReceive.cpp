#include <Arduino.h>
#include <Global_vars.h>
#include <pwm.h>
#include <Voice.h>
boolean receive_data_flag=false;//是否接受到信号
boolean remote_running=false;//是否在运行动作
int button_pressed=99;//123456前后左右平移7模式8清空9撤回10音量11运行13左前14右前23左后24右后
void DataProcess(char dataX, int dataY){//目前在espnow中处理接受的信息转化为按键信息，之后要替换成接收宇鑫的信息转化为按键
    
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
        }else if(dataX=='W'&&dataY==13){//左前组合
            button_pressed=13;
        }else if(dataX=='W'&&dataY==14){//右前组合
            button_pressed=14;
        }else if(dataX=='W'&&dataY==23){//左后组合
            button_pressed=23;
        }else if(dataX=='W'&&dataY==24){//右后组合
            button_pressed=24;
        }
        else if(dataX=='M'){//切换模式按键
            button_pressed=37;
        }else if(dataX=='C'){//清空指令按键
            button_pressed=38;
        }else if(dataX=='D'){//删除指令按键
            button_pressed=39;
        }
        else if(dataX=='V'){//音量控制按键
            button_pressed=40;
        }else if(dataX=='R'){//运行按键
            button_pressed=41;
        }
        else if(dataX=='0'){
            connected_with_controller=true;
        }

        if(button_pressed!=99){
            receive_data_flag=true;//如果有接收到有效按键，这个receive_data_flag置位1
            //Serial.print("button_pressed: ");
            //Serial.println(button_pressed);
        }
    }
}

void change_modes(){//切换不同的游戏模式
    Mode_Switch++;
    if(Mode_Switch>Max_Play_Modes){//
        Mode_Switch=0;
    }
    Serial.print("Current PlayMode: ");Serial.println(Mode_Switch);
}

void change_volumes(){
    if(Speaker_volume==15){Speaker_volume=0;pref.begin("volume",false);pref.putInt("vol",Speaker_volume);pref.end();change_volume(Speaker_volume);}
    else if(Speaker_volume==0){Speaker_volume=27;pref.begin("volume",false);pref.putInt("vol",Speaker_volume);pref.end();change_volume(Speaker_volume);play_voice(63);}//这个63号声音目前是滴的一声很不明显，要替换成不同挡位的语音提示
    else if(Speaker_volume==27){Speaker_volume=20;pref.begin("volume",false);pref.putInt("vol",Speaker_volume);pref.end();change_volume(Speaker_volume);play_voice(63);}
    else if(Speaker_volume==20){Speaker_volume=15;pref.begin("volume",false);pref.putInt("vol",Speaker_volume);pref.end();change_volume(Speaker_volume);play_voice(63);}
    Serial.print("Speeak_volume: ");Serial.println(Speaker_volume);
}

void Remote_Data_Analyse(){
    if(Mode_Switch==0){
        if(button_pressed<37)remote_control_behaviors();//在pwm.h做具体处理
    }

    //下面的遥控信息接收和游戏模式无关
    if(button_pressed==37)change_modes();
    else if(button_pressed==40)change_volumes();
    button_pressed=99;//处理完一次信息之后恢复未接收按键的状态
}
