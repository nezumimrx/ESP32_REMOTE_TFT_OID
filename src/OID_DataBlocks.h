#ifndef OID_DataBlocks_h
#define OID_DataBlocks_h

#include "Arduino.h"
class OID_DataBlocks
{
   public:
    OID_DataBlocks( );
    //单张卡片的边长是50*50个OID unit 对应1个unit相当于1.35mm 就是67.5mm
    //因此，除了铺点的地方，再加上2.5mm出血，就是72.5mm边长的卡片
    //所有卡片都基于X坐标8000，而Y坐标不同，所以数组里只存储Y坐标就行
    //为了保证识别率，Y坐标从第一张卡片结束到下一张卡片开始，间隔10
    //第一种条件卡片 对应{}
    int condition_type1[2] = { 50, 100};
    int condition_type2[2] = { 110, 160};
    int condition_type3[2] = { 170, 220};
    //遥控模式场地卡
    int trap_card_speedup[2] = {230,280};//加速卡
    int trap_card_slowdown[2]= {290,340};//减速卡
    int trap_card_turn[2]= {350,400};//掉头
    int trap_card_confuse[2]= {410,460};//混乱卡
    int trap_card_stop[2]={470,520};//停顿卡
    int trap_card_winning[2]= {530,580};//胜利卡
    //遥控模式指令卡
    int trap_card_forward[2]= {590,640};//前进卡
    int trap_card_left[2]= {650,700};//左转卡
    int trap_card_right[2]= {710,760};//右转卡
    int trap_card_loop2[2]={770,820};//循2次环卡
    int trap_card_loop3[2]={830,880};//循3次环卡

    int trap_card_condition[2]={890,940};//条件卡
    //闯关\积分模式场地卡
    int code_1_card_1[2]={950,1000};//第1关开始
    int code_1_card_2[2]={1010,1060};//第1关结束，第2关开始
    int code_1_card_3[2]={1070,1120};//第2关结束，第3关开始
    int code_1_card_4[2]={1130,1180};//第3关结束，第4关开始
    int code_1_card_5[2]={1190,1240};//第4关结束，第5关开始
    int code_1_card_6[2]={1250,1300};//第5关结束，第6关开始
    int code_1_card_7[2]={1310,1360};//第6关结束，第7关开始
    int code_1_card_8[2]={1370,1420};//第7关结束，第8关开始
    int code_1_card_9[2]={1430,1480};//第8关结束，第9关开始
    int code_1_card_10[2]={1490,1540};//第9关结束，通关
    int code_trap_card_type1[2]={1550,1600};//编程、生存模式陷阱卡1，对应不同语音
    int code_trap_card_type2[2]={1610,1660};//编程、生存模式陷阱卡2，对应不同语音
    int code_trap_card_type3[2]={1670,1720};//编程、生存模式陷阱卡3，对应不同语音
    int code_trap_card_type4[2]={1730,1780};//编程、生存模式陷阱卡4，对应不同语音
    int code_trap_card_type5[2]={1790,1840};//编程、生存模式陷阱卡5，对应不同语音
};

#endif





















