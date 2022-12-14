#include <Arduino.h>
#include <Global_vars.h>
#include <pwm.h>
int my_face_direction=0;//当前小车在地图上的朝向，1-朝上(y负方向)2朝下3朝左4朝右，每个关卡开始时，需要设定这个变量
int Stage_Start_Point_X=int((Story_StartPoint_X_end-Story_StartPoint_X_start)/2);//在执行用户指令前，先调整小车到起始格子中心，这个变量就是X坐标
int Stage_Start_Point_1_Y= int((Story_StartPoint_Y_start+oid_cell_unit_size)/2);//这是第一关起始格子的纵坐标，之后每关根据关数递增格子的大小

//开始执行用户命令前先调整自己位置到格子中心，输入当前是第几关Stage_Level，这个关卡起始朝向Stage_Start_Direction就可以
void self_reposition_to_center_of_startcell(int Stage_Level,int Stage_Start_Direction){
    int start_angle=0;
    if(Stage_Start_Direction==1)start_angle=0;
    else if(Stage_Start_Direction==2)start_angle=180;
    else if(Stage_Start_Direction==3)start_angle=270;
    else if(Stage_Start_Direction==4)start_angle=90;
    if(pwm_oid_reposition(Stage_Start_Point_X,(Stage_Start_Point_1_Y+(on_Story_Stage_Level-1)*oid_cell_unit_size),start_angle,255)){//第一关，小车朝上
            Serial.println("On starting Point");
            my_face_direction=Stage_Start_Direction;//这个很重要，是执行地图编程时，每关开始时候在开始格子里的方向
    } 
}

void Story_Stage_Preparation(){//关卡前置背景、位置调整、动画表情和音乐等
    if(on_Story_Stage_Level==1){
        Serial.println("Story_Stage_1");
        //self-reposition 先调整自己的朝向
        self_reposition_to_center_of_startcell(1,1);
    }
    else if (on_Story_Stage_Level==2)
    {
        Serial.println("Story_Stage_2");
        //self-reposition 先调整自己的朝向
        self_reposition_to_center_of_startcell(2,1);
           
    }
    else if (on_Story_Stage_Level==3)
    {
        Serial.println("Story_Stage_3");
        self_reposition_to_center_of_startcell(3,1);    
    }
    else if (on_Story_Stage_Level==4)
    {
        Serial.println("Story_Stage_4");
        self_reposition_to_center_of_startcell(4,1);    
    }
    
}

void Story_Wrong_Start_Pos(){//提示用户放错位置了，无法解析代码或自己调整位置
    Serial.println("Car not on Start point");
}

void check_whether_on_story_stage(){//检查用户是否放在了地图的关卡开始格子上
    if(on_Story_Stage_Level!=0){//看看是不是踩到了故事地图的起始点上，如果是，那么会使在Code_parse()中调用的process_wheel切换成pwm_oid_movement
        process_based_on_OID_coordinates=true;//这里
        able_to_cypher=0;//满足解析条件
        //播放对应关卡故事背景语音、表情、动画，&&&&&&&&&&&重要，记得调整初始朝向，并传递给CodeParse.cpp里的previous_direction
        Story_Stage_Preparation();

    }
    else if(on_Story_Stage_Level==0&&on_coding_map==1){//虽然没踩在起始点，但是踩在了故事地图上，说明用户把小车放错位置了
        //提示用户放错位置了，无法解析代码或自己调整位置
        Story_Wrong_Start_Pos();
        able_to_cypher=1;//想要解析代码，但是起始位置放错了，在故事地图上但是没在起始点位置
        
    }else if(on_Story_Stage_Level==0&&on_coding_map==0){
        //没有检测到地图，也没有检测到站在起始点上
        process_based_on_OID_coordinates=false;
        able_to_cypher=0;//可以解析，但是不按照OID识别来进行
        //播放摆错位置提示，并提示用户把小车放到指定位置
    }
}