#ifndef _CodeParse_H_
#define _CodeParse_H_
#include <Arduino.h>
//在Check Code完成之后，start_cypher置位1之后，处理编程里指令
void code_parse(String str);
void instant_stop_funcs();
void code_parse_emergent_stop();
#endif
