
#ifndef _Espnow_slave_H_
#define _Espnow_slave_H_
#include <Arduino.h>
const uint8_t masteraddress[]={0xA0, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA};
const uint8_t myaddress[]={0xA0, 0x01, 0xAA, 0xAA, 0xAA, 0xAA};
void espnow_slave_init();
void send_data_now(char c,int num);
void lost_connection_funcs();
void build_connection_funcs();
#endif