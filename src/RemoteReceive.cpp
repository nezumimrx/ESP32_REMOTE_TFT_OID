#include <Arduino.h>
#include <Global_vars.h>
#include <pwm.h>
#include <Voice.h>
#include <Command_Analyse.h>
#include <CodeParse.h>
boolean receive_data_flag=false;//是否接受到信号
boolean remote_running=false;//是否在运行动作
int button_pressed=88;//123456前后左右平移13左前14右前23左后24右后/25循环2次/26循环3次/27循环结束/28条件1开始/29条件1结束/30条件2开始/31条件2结束/32条件3开始/33条件3结束/34交互1/35交互2/36交互3/37模式38清空39撤回40音量41运行
                      //其中button_pressed变为25-36之间的数字并不是由遥控器输入的，而是由用户读取卡片从而实现的，例如用户录入指令阶段，读取了一张场景交互卡1，则同样会触发button_pressed=34
int previous_button_pressed=88;
boolean button_pressed_timer_start=false;
int button_pressed_timer_counter=0;
void multiple_data_received_filter();
void Remote_Data_Analyse();
void DataProcess(char dataX, int dataY){//目前在espnow中处理接受的信息转化为按键信息，之后要替换成接收宇鑫的信息转化为按键
    button_pressed=88;
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
        
        multiple_data_received_filter();
        //button_pressed=88;//这里不能恢复成88，因为如果这里立马回复了，循环中还没有使用过button_pressed，则会表现为按键失灵，所以要在Remote_Data_Analyse()末尾恢复为88才行
        Remote_Data_Analyse();//必须和接收消息是统一的速率，否则信息会误解析。例如此时connected_with_controller=true;只在onDataReceive运行了1次，但是在循环里已经跑了N多次
    }
}

void multiple_data_received_filter(){
    //过滤多次重复接收到的信息
    //思路：首先由于数据是在短时间内重复接收到的，所以要判断是否为重复数据有2个条件
    //1.前一个数据和当前数据是否不同
    //2.当前数据距离上一次接收到隔了多长时间，如果间隔时间很长，说明数据不属于重复接收到的情况，而是用户发送了2次数据。
    //第1种情况很简单，只要判断上一个数据和当前数据是否相等即可
    //第2种情况需要用一个timer来计时，每次接收到数据，timer就开始计时，同时timer清零表示仍然有数据在接收。当timer超过一定时间，自动停止计时，说明数据中断了。此时信息就被过滤掉了。
    //最后一点需要注意的是，不能在接受完数据之后立马恢复用于记录的变量，如这个button_pressed不能在这个filter里恢复为88，要在使用过这个数据之后才恢复。也就是要在Remote_Data_Analyse()末尾恢复为88才行

    if(previous_button_pressed!=button_pressed){//过滤接收到的重复数据，如果之前的按键和当前按键不同，则为有效数据。那如果两次都按的同一个按键呢？则需要通过判断未接收到信息的时间，如果连续250ms未接收到信息，则在TimerTask里恢复previous_button为88
        
        Serial.print("button: ");Serial.println(button_pressed);//这里可能会显示出88，但是不影响，只是循环的速度大于了接收的速度瞬时进行了改变
        receive_data_flag=true;//接收到了

    }
        
    previous_button_pressed=button_pressed;
    button_pressed_timer_start=true;
    button_pressed_timer_counter=0;
    //button_pressed=88;//这里不能恢复成88，因为如果这里立马回复了，循环中还没有使用过button_pressed，则会表现为按键失灵，所以要在Remote_Data_Analyse()末尾恢复为88才行
    
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
        if(button_pressed<37&&button_pressed>0&&receive_data_flag==true){remote_control_behaviors(button_pressed);remote_running=true;receive_data_flag=false;}//在pwm.h做具体处理
        else if(button_pressed==0&&receive_data_flag==true){remote_control_behaviors(button_pressed);remote_running=false;receive_data_flag=false;}
        else if(button_pressed==37&&receive_data_flag==true){change_modes();receive_data_flag=false;}
        else if(button_pressed==40&&receive_data_flag==true){change_volumes();receive_data_flag=false;}

    }else if(Mode_Switch==1){
        if(start_cypher==false){//没有开始运行编程指令，用户可以对指令进行编辑，一旦开始，只能紧急停止
            if(button_pressed<37&&button_pressed!=0&&receive_data_flag==true){add_remote_commands();receive_data_flag=false;}//录入编程指令,button_pressed!=0保证动作按键连带的00 0000 0000不会被计入
            else if(button_pressed==37&&receive_data_flag==true){change_modes();receive_data_flag=false;}
            else if(button_pressed==40&&receive_data_flag==true){change_volumes();receive_data_flag=false;}

            else if(button_pressed==38&&receive_data_flag==true){clear_commands();receive_data_flag=false;}//清空指令
            else if(button_pressed==39&&receive_data_flag==true){delete_commands();receive_data_flag=false;}//撤回指令
            else if(button_pressed==41&&receive_data_flag==true){run_commands();receive_data_flag=false;}//运行，起来接着写

        }else if(start_cypher==true){//开始解析编程指令时，用户不能编辑指令，可以紧急停止或切换模式的同时紧急停止，
            if(button_pressed==41&&receive_data_flag==true){
                //用户再次按下运行按钮，紧急停止，允许这个操作
                code_parse_emergent_stop();
                receive_data_flag=false;
            }else if(button_pressed==37&&receive_data_flag==true){
                //用户紧急切换模式，允许这个操作
                code_parse_emergent_stop();
                receive_data_flag=false;
                change_modes();
            }
        }
    }

    //下面的遥控信息接收和游戏模式无关
    button_pressed=88;//Remote_Data_Analyse()使用完数据，恢复88
}




