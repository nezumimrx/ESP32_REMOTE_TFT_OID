#ifndef _OID_Card_Read_H_
#define _OID_Card_Read_H_
#include <Arduino.h>
void Check_on_coding_map();
void Card_Data_Analyse();
void Mask_StartCell_StoryMode();//将读取到的OID_X映射到MaskOID_X
#endif