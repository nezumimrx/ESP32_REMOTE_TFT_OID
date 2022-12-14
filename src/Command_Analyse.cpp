#include <Arduino.h>
#include <Global_vars.h>
#include <Voice.h>
#include <CheckCode.h>
#include <StoryPreparation.h>

int commands_array[commands_size]={0};//commands_size是最大能容纳的指令数，这个在GLOBAL_VARS.H中define
int commands_counter=0;//记录当前录入了多少个指令
//0-可以解析，
//1-想要解析代码，但是起始位置放错了，在故事地图上但是没在起始点位置
//2第二种不满足条件...某些情况不允许用户解析编程指令，例如遇到编程挑战遇到陷阱，而用户再次按下了run按钮，则无法解析
uint8_t able_to_cypher=0;

void add_remote_commands(){//添加指令，在Remote_Data_Analyse()中调用
if(commands_counter<commands_size){
        //将用户录入的指令添加到commands_array里，每添加1个数据，整个数组向后移1位，例如已有数据[1,2,3,...]新添加4进来，则[4,1,2,3,...]
        for(int i=(commands_size-1);i>=1;i--){
            commands_array[i]=commands_array[i-1];
        }
        commands_array[0]=button_pressed;
        //把指令添加到code_str_raw字符串里
        if(button_pressed==1){code_str_raw+=";W1";play_voice_condition=1;need_play_voice=true;}
        else if(button_pressed==2){code_str_raw+=";W2";play_voice_condition=1;need_play_voice=true;}
        else if(button_pressed==3){code_str_raw+=";W3";play_voice_condition=1;need_play_voice=true;}
        else if(button_pressed==4){code_str_raw+=";W4";play_voice_condition=1;need_play_voice=true;}
        else if(button_pressed==5){code_str_raw+=";W5";play_voice_condition=1;need_play_voice=true;}
        else if(button_pressed==6){code_str_raw+=";W6";play_voice_condition=1;need_play_voice=true;}
        else if(button_pressed==25){code_str_raw+=";(2";play_voice_condition=11;need_play_voice=true;}
        else if(button_pressed==26){code_str_raw+=";(3";play_voice_condition=11;need_play_voice=true;}
        else if(button_pressed==27){code_str_raw+=";)";play_voice_condition=12;need_play_voice=true;}
        else if(button_pressed==28){code_str_raw+=";{";play_voice_condition=9;need_play_voice=true;}
        else if(button_pressed==29){code_str_raw+=";}";play_voice_condition=10;need_play_voice=true;}
        else if(button_pressed==30){code_str_raw+=";[";play_voice_condition=9;need_play_voice=true;}
        else if(button_pressed==31){code_str_raw+=";]";play_voice_condition=10;need_play_voice=true;}
        else if(button_pressed==32){code_str_raw+=";<";play_voice_condition=9;need_play_voice=true;}
        else if(button_pressed==33){code_str_raw+=";>";play_voice_condition=10;need_play_voice=true;}
        else if(button_pressed==34){code_str_raw+=";F1";}//play_voice_condition=?;need_play_voice=true;}目前还没有对应场景交互的声音
        else if(button_pressed==35){code_str_raw+=";F2";}//play_voice_condition=?;need_play_voice=true;}目前还没有对应场景交互的声音
        else if(button_pressed==36){code_str_raw+=";F3";}//play_voice_condition=?;need_play_voice=true;}目前还没有对应场景交互的声音
        Serial.print("RAW INPUT Commands: ");Serial.println(code_str_raw);
        //在屏幕上绘制出用户录入的最新的4个指令 指令内容为commands_array[0],commands_array[1],commands_array[2],commands_array[3]
        /*
        if(commands_array[0]!=0)drawSymbol(commands_array[0],posX,posY);
        if(commands_array[0]!=0)drawSymbol(commands_array[1],posX,posY);
        if(commands_array[0]!=0)drawSymbol(commands_array[2],posX,posY);
        if(commands_array[0]!=0)drawSymbol(commands_array[3],posX,posY);
        */
        //当前记录的指令数+1
        commands_counter++;
        Serial.print("commands_counter: ");Serial.println(commands_counter);
    }else if(commands_counter>=commands_size){//超出了最大指令数量
        //播放声音提示用户
        need_play_voice=true;
        play_voice_condition=random(5,8);//程序OVERSIZE
        Serial.println("Commands oversize!");
    }
}

String delete_last_item()
{
    String deleted_str = "";
    int last_delimiter_index = code_str_raw.lastIndexOf(";");
    deleted_str = code_str_raw.substring(0, last_delimiter_index);
    return deleted_str;
}

void delete_commands(){//撤回，在Remote_Data_Analyse()中调用
    if(commands_counter==0){//已经没有指令可以撤回了
        for(int i=0;i<commands_size;i++)commands_array[i]=0;
        code_str_raw="&";
        play_voice_condition=4;//清空所有指令语音
        need_play_voice=true;
        Serial.println("No more commands to delete!");
    }else{//指令数量大于0，可以清除
        for(int i=0;i<(commands_size-1);i++)commands_array[i]=commands_array[i+1];
        code_str_raw = delete_last_item();
        //在屏幕上绘制出用户录入的最新的4个指令 指令内容为commands_array[0],commands_array[1],commands_array[2],commands_array[3]
        /*
        if(commands_array[0]!=0)drawSymbol(commands_array[0],posX,posY);
        if(commands_array[0]!=0)drawSymbol(commands_array[1],posX,posY);
        if(commands_array[0]!=0)drawSymbol(commands_array[2],posX,posY);
        if(commands_array[0]!=0)drawSymbol(commands_array[3],posX,posY);
        */
        commands_counter--;
        Serial.print("Deleted RAW: ");Serial.println(code_str_raw);
        Serial.print("command_counter: ");Serial.println(commands_counter);
    }
}

void clear_commands(){//清除，在Remote_Data_Analyse()中调用
    code_str_raw="&";
    commands_counter=0;
    for(int i=0;i<commands_size;i++)commands_array[i]=0;
    need_play_voice=true;
    play_voice_condition=random(12,15);//清空指令语音

}

void run_commands(){
    
    check_whether_on_story_stage();//在执行用户输入的指令前，先判断一下是否在地图上如果在，那么on_Story_Stage_Level!=0且process_based_on_OID_coordinates=true;
    if(able_to_cypher==0){//满足解析条件

        check_code(code_str_raw);//看看用户输入的指令，最终会反应在start_cypher这个变量里，从而使线程Code_Process_TASK开始运行

    }else {

    }
}






