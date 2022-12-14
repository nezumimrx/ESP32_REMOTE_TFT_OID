#include <Arduino.h>
#include <Global_vars.h>
#include <pwm.h>
#include <Voice.h>
#include <Command_Analyse.h>
#include <CodeParse.h>
boolean receive_data_flag=false;//是否接受到信号
boolean remote_running=false;//是否在运行动作
int button_pressed=99;//123456前后左右平移13左前14右前23左后24右后/25循环2次/26循环3次/27循环结束/28条件1开始/29条件1结束/30条件2开始/31条件2结束/32条件3开始/33条件3结束/34交互1/35交互2/36交互3/37模式38清空39撤回40音量41运行
                      //其中button_pressed变为25-36之间的数字并不是由遥控器输入的，而是由用户读取卡片从而实现的，例如用户录入指令阶段，读取了一张场景交互卡1，则同样会触发button_pressed=34

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

void Remote_Data_Analyse(){//包含解析代码，但是处理编程指令在Code_Process_TASK里，这样可以接收用户紧急停止的指令
    if(Mode_Switch==0){//遥控模式
        if(button_pressed<37&&button_pressed>0){remote_control_behaviors(button_pressed);remote_running=true;}//在pwm.h做具体处理
        else if(button_pressed==0){remote_control_behaviors(button_pressed);remote_running=false;}
        else if(button_pressed==37)change_modes();
        else if(button_pressed==40)change_volumes();

    }else if(Mode_Switch==1){
        if(start_cypher==false){//没有开始运行编程指令，用户可以对指令进行编辑，一旦开始，只能紧急停止
            if(button_pressed<37&&button_pressed!=0)add_remote_commands();//录入编程指令,button_pressed!=0保证动作按键连带的00 0000 0000不会被计入
            else if(button_pressed==37)change_modes();
            else if(button_pressed==40)change_volumes();

            else if(button_pressed==38)clear_commands();//清空指令
            else if(button_pressed==39)delete_commands();//撤回指令
            else if(button_pressed==41)run_commands();//运行，起来接着写

        }else if(start_cypher==true){//开始解析编程指令时，用户不能编辑指令，可以紧急停止或切换模式的同时紧急停止，
            if(button_pressed==41){
                //用户再次按下运行按钮，紧急停止，允许这个操作
                code_parse_emergent_stop();
            }else if(button_pressed==37){
                //用户紧急切换模式，允许这个操作
                code_parse_emergent_stop();
                change_modes();
            }
        }
    }

    //下面的遥控信息接收和游戏模式无关
    
    button_pressed=99;//处理完一次信息之后恢复未接收按键的状态
}




