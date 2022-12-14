#ifndef _Command_Analyse_H_
#define _Command_Analyse_H_
#include <Arduino.h>
//在DataReceive到遥控器命令之后，处理遥控器命令，特别是编程指令
void add_remote_commands();
void clear_commands();
void delete_commands();
void run_commands();
#endif