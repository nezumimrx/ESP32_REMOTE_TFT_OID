#include <Arduino.h>
#include <Global_vars.h>

Preferences pref;
int volume=15;
/*
目前用的仍然是串口通信的语音芯片，之后肯定要换成雨鑫的语音芯片

*/

/*输入音频数字播放的基础功能，这个替换语音芯片的话要替换掉
*/
void play_voice(int num)
{
  byte playmusic[] = {0x7E, 0x05, 0x41, 0x00, 0x00, 0x45, 0xEF};
  byte highbyte = highByte(num);
  byte lowbyte = lowByte(num);
  byte confirmbyte = 0x05 ^ 0x41 ^ highbyte ^ lowbyte;
  playmusic[3] = highbyte;
  playmusic[4] = lowbyte;
  playmusic[5] = confirmbyte;
  Serial.write(playmusic, sizeof(playmusic));
  Serial.write("\r\n");
}
/*输入数字改变音量的基础功能，这个随语音芯片替换要替换
*/
void change_volume(int num){//0-30音量
    byte level = lowByte(num);
    byte changevolume[] = {0x7E, 0x04, 0x31, 0x19, 0x2C, 0xEF};
    byte confirmbyte= 0x04 ^ 0x31 ^ level;
    changevolume[3]=level;
    changevolume[4]=confirmbyte;
    Serial.write(changevolume,sizeof(changevolume));
    Serial.write("\r\n");
}

/*输入数字改变音量的基础功能，这个随语音芯片替换要替换
*/
void volume_read_memory(){
  pref.begin("volume",false);//false-write/read true-read only
  int read_volume = pref.getInt("vol",0);//读取nvm存储的音量是多少
  if(read_volume==0)change_volume(volume);//如果是0，说明没改过设置，默认按0-30的第15档设置
  else change_volume(read_volume);//如果不是0，那么用户更改过音量设置，按上一次保存的音量运行
  pref.end();
}
