#include <Arduino.h>
#include <Global_vars.h>
#include <pwm.h>
#include "CodeParse.h"
String looptimes_strFormat = ""; //这个looptimes_strFormat由于放在下面开空间的话会出现存不进去数据的情况，所以要放到全局变量中
boolean processing_condition =false;
void code_parse_emergent_stop(){
    instant_stop=1;
    start_cypher=0;
    vTaskSuspend(Code_Process_Handle);
    code_str_clean="";
    pwm_stop();
}


void step_on_condition_card_play_voice(boolean &receive_condition_type, boolean &has_condition_type, boolean &has_other_condition_1, boolean &has_other_condition2, String &temp_condition_str){
    //踩到哪个卡片了？有没有这个卡片的指令？有没有其他卡片的指令？指令是什么？做出语音提示，有指令且执行，有指令但不是踩到的这个卡片，啥指令都没有
    if(receive_condition_type==1&&has_condition_type==1&&instant_stop==false&&processing_condition==false){
        Serial.println("step on condition1 card,now proceed");
        receive_condition_type=0;
        //让小车停下
        pwm_stop();
        //播放声音，触发第一种条件卡.给3秒时间播放语音
        //..voice_condition=15;vTASKDelay(4000/porTICK_PERIOD_MS);
        vTaskDelay(4000/portTICK_PERIOD_MS);
        processing_condition=true;
        code_parse(temp_condition_str);
        processing_condition=false;
        Serial.println("finish condition commands");  
        }else if(receive_condition_type==1&&has_condition_type==0&&instant_stop==false&&(has_other_condition_1!=0||has_other_condition2!=0)){
        Serial.println("step on condition card, but no commands");
        receive_condition_type=0;
        pwm_stop();//让小车停下
        //播放声音,触发了条件但是没有录入对应的条件指令
        //..
        vTaskDelay(4000/portTICK_PERIOD_MS);
        }else if(receive_condition_type==1&&has_condition_type==0&&instant_stop==false&&has_other_condition_1==0 && has_other_condition2==0){
        Serial.println("no condition commands whatsoever");
        receive_condition_type=0;
        pwm_stop();
        //播放声音，触发了条件但是没有录入任何条件指令
        //..
        vTaskDelay(4000/portTICK_PERIOD_MS);
    }
}

void process_wheel(String str, int i)
{ // L0-stop L1-forward L2-BACK L3-LEFT L4-RIGHT L5-MECLEFT L6-MECRIGHT 
  // L001 L101 L201 01表示时间
  int move_condition=str.substring(i+1,i+2).toInt();
  char next_next_char = str[i + 2];
  //第一种情况，如果后面带上数字卡片表示时间了，例如前进卡片W1 加上数字卡片02 行程了W102的情况，那么判断next_next_char是0成立的话，就执行下面的运算
  if(next_next_char=='0'){
    int delay_time=str.substring(i+2,i+4).toInt();
    //send_data_now('W',move_condition);
    basic_movements(move_condition,Mode_1_Speed);
    vTaskDelay((500*delay_time)/portTICK_PERIOD_MS);
    //delay(500*delay_time);
    pwm_stop();
    vTaskDelay(200/portTICK_PERIOD_MS);
    //delay(200);
    i=i+3;
  }
  //如果后面没有时间，只是单纯的W1\W2\W3\W4\W5\W6的话，停顿1秒
  else if(next_next_char!='0'){
    basic_movements(move_condition,Mode_1_Speed);
    if(move_condition==3)vTaskDelay(320/portTICK_PERIOD_MS);//旋转时间
    else if(move_condition==4)vTaskDelay(320/portTICK_PERIOD_MS);//旋转时间
    else vTaskDelay(230/portTICK_PERIOD_MS);//直行时间
    
    pwm_stop();
    vTaskDelay(200/portTICK_PERIOD_MS);
    i++;
  }

}

void oid_wheel(String str, int i){
    int move_direction=str.substring(i+1,i+2).toInt();
    if(move_direction==4)move_direction=2;//W4是右转，但是move_direction=2才是右转，这里是小失误，忘了W2是后退了,但是编程模式下没有W2
    
    // Serial.print("my_face_direction: ");Serial.println(my_face_direction);
    // Serial.print("move_direction: ");Serial.println(move_direction);
    if(pwm_oid_movement(my_face_direction,move_direction)){
        if(move_direction==1)my_face_direction=my_face_direction;
        else {
            if(my_face_direction==1&&move_direction==2)my_face_direction=4;//原来朝上，现在右拐，朝右
            else if(my_face_direction==1&&move_direction==3)my_face_direction=3;//原来朝上，现在左拐，朝左
            else if(my_face_direction==2&&move_direction==2)my_face_direction=3;//原来朝下，现在右转，朝左
            else if(my_face_direction==2&&move_direction==3)my_face_direction=4;//原来朝下，现在左转，朝右
            else if(my_face_direction==3&&move_direction==2)my_face_direction=1;//做来朝左，现在右转，朝上
            else if(my_face_direction==3&&move_direction==3)my_face_direction=2;//原来朝左，现在左转，朝下
            else if(my_face_direction==4&&move_direction==2)my_face_direction=2;//原来朝右，现在右转，朝下
            else if(my_face_direction==4&&move_direction==3)my_face_direction=1;//原来朝右，现在左转，朝上
        }
        Serial.println("one move finished");
    }else {
        Serial.println("something wrong cannot finish one move,stop!");
        code_parse_emergent_stop();
    }

}

boolean process_D(char next_char, char next_next_char, int i)
{

  if (next_char <= 57 && next_char >= 48)
  {
    int next_char_int = next_char - '0';
    int next_next_char_int = next_next_char - '0';
    int delay_time = next_char_int * 10 + next_next_char_int;
    Serial.print(F("delay: "));
    Serial.println(delay_time);
    vTaskDelay((delay_time * 1000)/portTICK_PERIOD_MS);
    return 1;
  }
  else
  {
    Serial.println("default delay 1 ");
    vTaskDelay(1000/portTICK_PERIOD_MS);
    return 0;
  }
}

void code_parse(String str){
    int str_length=str.length();
    int header = int(str[0]);
    int looptimes=1;
    if(header<57&&header>=48){
        //首字母时数字开头所以是循环
        looptimes_strFormat="";
        for(int i=0;i<str_length;i++){
            int current_ASCII_number = int(str[i]);
            if(current_ASCII_number<=57&&current_ASCII_number>=48){
                //读到的下一位还是数字，那么就添加到looptimes_strFormat里，例如12W01,1肯定是确定了，而2跟在1后面，所以添加
                looptimes_strFormat +=(current_ASCII_number-'0');
            }else if(current_ASCII_number>65||current_ASCII_number<=47){
                //读到A-z的字符，结束读取循环数字
                break;
            }
        }
        looptimes = looptimes_strFormat.toInt();
        if(looptimes==0)looptimes=2;//当格式为&(0DD)的时候表示用户只放了loop卡片但是没有加任何数字卡片，组合成&(02DD)这种格式，所以默认循环2次
    }
    for(int j=0;j<looptimes;j++){
        for(int i=0;i<str_length;i++){
            char ch =str[i];
            if(instant_stop==true){//用户发送了紧急停止命令
                i=str_length;
                j=looptimes;
                break;//结束循环
            }else{
                step_on_condition_card_play_voice(receive_condition_type1,has_condition_type1,has_condition_type2,has_condition_type3,code_str_condition_type1);
                step_on_condition_card_play_voice(receive_condition_type2,has_condition_type2,has_condition_type1,has_condition_type3,code_str_condition_type2);
                step_on_condition_card_play_voice(receive_condition_type3,has_condition_type3,has_condition_type1,has_condition_type2,code_str_condition_type3);
                /*生存模式要让小车停下来播放语音
                if(survive_step_on_card==true){
                    Serial.println("Survive step on point card");
                    pwm_stop();
                    vTaskDelay(5000/portTICK_PERIOD_MS);
                    survive_step_on_card=false;
                }
                */
                if(ch=='('){
                    String loopString = str.substring(i + 1); //把遇到循环起始字符之后的全部截下来
                    //如果遇到了起始符'('，而且subString有终止字符')'，那么我们要继续循环分析的loopString中直到第一次遇到')'所在的位置就都要跳过，因此indexEnd要在i所在位置的基础上+loopString.indexOf(')')
                    //但是当遇到起始字符'('，但没有遇到')'，说明没有再嵌套循环，因此跳过整句loopString的长度就可以 例如(4W2)(3W2) 截取到的loopString是4W2)(3W2),因为还有(3W2)这个循环在所以就不能跳过整个loopString
                    //例如(2A(3B)C)(4D), loopString=2A(3B)C)(4D)  例如(4D) 例如&(2D02(2D01(3D))D04)
                    int indexofNegativeBracket = loopString.indexOf(')'); //第一次出现)的位置 在2A(3B)C)(4D)中,indexofNegativeBracket=5 在4D)中 indexofNegativeBracket=2 2D02(2D01(3D))D04)
                    int indexEndSignal = 0;
                    if (indexofNegativeBracket == (loopString.length() - 1)){ 
                        //例如出现了2D01(3D) 的情况，3D)的反括弧已经是最后一位了所以没办法再提取下一个negativeSubstring
                        indexEndSignal = loopString.length();
                    }else{
                        String negativeSubstring = loopString.substring(indexofNegativeBracket + 1); // C)(4D)
                        int nextPositiveIndex = negativeSubstring.indexOf('(');                      //在C)(4D)中nextPositiveIndex=2
                        if (nextPositiveIndex > 0)indexEndSignal = indexofNegativeBracket + nextPositiveIndex; //对于2A(3B)C)(4D)就是4D前面的(位置
                        else if (nextPositiveIndex < 0)indexEndSignal = loopString.lastIndexOf(')');
                    }

                    if (indexEndSignal > 0){
                        String nextLevelLoop = loopString.substring(0, indexEndSignal);
                        Serial.println(nextLevelLoop);
                        code_parse(nextLevelLoop);
                        i = i + indexEndSignal;
                    }else if (indexEndSignal < 0){
                        code_parse(loopString);
                        i = i + loopString.length();
                    }
                }

                else if(ch =='W'){
                    if(process_based_on_OID_coordinates)oid_wheel(str,i);
                    else process_wheel(str,i);   
                }
                else if(ch =='D'){
                    char next_char = str[i + 1];
                    char next_next_char = str[i + 2];
                    if (process_D(next_char, next_next_char, i))i = i + 2;
                    else i = i;
                }

            }
            
        }
    }
}












