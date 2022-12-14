#include <Arduino.h>
#include <Global_vars.h>
boolean DEBUG2=0;
boolean on_starting_cell=false;//是否在开始格
//编程地图关卡的起始坐标,OID坐标每+1，对应长度增加1.4，暂定一个格子是79.8mm也就是57个单位坐标
//规定，关卡的起始格子X坐标从5000开始到5057，大小80mm
//规定，关卡的起始格子Y坐标从
int on_Story_Stage_Level=0;

void Check_on_coding_map(){
    if(OID_available){//可能在地图上，还要判断下坐标
        if(OID_X>=map_start_X&&OID_X<=map_end_X&&OID_Y>=map_start_Y&&OID_Y<=map_end_Y){
            on_coding_map=true;
            off_coding_map_test_start=false;//不用计时检测是否离开了地图
            off_coding_map_counter=0;//重置计时次数
        }
    }else{//如果on_coding_map还是true，但现在OID_available不是true不在地图上,则在TickTask中开始计时，如果这段时间一直都没有成为OID_available以及在坐标范围内，那就是离开了地图
        if(on_coding_map==true&&off_coding_map_test_start==false){
            off_coding_map_test_start=true;//->TICKTASK开始计时
            
        }
    }
}

void Card_Data_Analyse(){//在主循环中判断踩到了什么卡
    if(OID_available){
        //故事关卡开始格子，识别出是第几关，on_Story_Stage_Level
        if(OID_X>=Story_StartPoint_X_start&&OID_X<=Story_StartPoint_X_end&&OID_Y>=Story_StartPoint_Y_start&&OID_Y<=Story_StartPoint_Y_end){//在某个故事地图的起始点上
            on_coding_map=true;//也在地图上
            on_Story_Stage_Level = int(floor((OID_Y-Story_StartPoint_Y_start)/oid_cell_unit_size))+1;//例如OID_Y是630那么(630-500)/57 ~ 2.28 floor之后是2.0再取整是2，因为这样只是是从0关开始，所以要再+1，才是从第一关开始，
            off_coding_map_test_start=false;//不用计时检测是否离开了地图
            off_coding_map_counter=0;//重置计时次数
            //Serial.print("Story_Stage_Level: ");Serial.println(on_Story_Stage_Level);
        }else {//没在任何起始点上
            Masked_OID_X=0;
            Masked_OID_Y=0;
            on_Story_Stage_Level=0;
        }

    }
}

void Mask_StartCell_StoryMode(){
    //由于地图编程卡片的起始格子为了判断是哪关，所以会被替换掉，这时候为了恢复成原本的坐标系进行pwm_calculate_movement，所以代入的变量是MaskedOIDX而不是OID_X
    if(on_Story_Stage_Level!=0){
        int Start_Cell_On_Map_X = 0;//开始格子在地图上的哪个位置？
        int Start_Cell_On_Map_Y = 0;
        if(on_Story_Stage_Level==1){Start_Cell_On_Map_X=Story_Level_1_X;Start_Cell_On_Map_Y=Story_Level_1_Y;}//第一关的开始格子左上角实际上是地图哪个坐标
        else if(on_Story_Stage_Level==2){Start_Cell_On_Map_X=Story_Level_2_X;Start_Cell_On_Map_Y=Story_Level_2_Y;}
        else if(on_Story_Stage_Level==3){Start_Cell_On_Map_X=Story_Level_3_X;Start_Cell_On_Map_Y=Story_Level_3_Y;}
        else if(on_Story_Stage_Level==4){Start_Cell_On_Map_X=Story_Level_4_X;Start_Cell_On_Map_Y=Story_Level_4_Y;}
        Masked_OID_X = OID_X-Story_StartPoint_X_start+Start_Cell_On_Map_X;//第一关的开始格子左上角实际上是地图哪个坐标
        Masked_OID_Y = OID_Y-Story_StartPoint_Y_start-(oid_cell_unit_size*(on_Story_Stage_Level-1))+Start_Cell_On_Map_Y;//第一关的开始格子左上角实际上是地图哪个坐标
        if(DEBUG2)Serial.print("Masked_OID_X: ");Serial.print(Masked_OID_X);
        if(DEBUG2)Serial.print(" Masked_OID_Y: ");Serial.println(Masked_OID_Y);
    }else {
        if(on_coding_map){
            Masked_OID_X=OID_X;
            Masked_OID_Y=OID_Y;
            if(DEBUG2)Serial.print("Masked_OID_X: ");Serial.print(Masked_OID_X);
            if(DEBUG2)Serial.print(" Masked_OID_Y: ");Serial.println(Masked_OID_Y);
        }  
    }
}
