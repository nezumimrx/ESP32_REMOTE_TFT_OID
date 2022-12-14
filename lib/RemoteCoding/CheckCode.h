#ifndef _CheckCode_H_
#define _CheckCode_H_
#include <Arduino.h>
//在Command_Anaylse之后，分析完自身处于那种Mode_Switch，开始接收编程指令之后，检查用户指令列是否正确
void check_code(String code_str_raw_local);
#endif