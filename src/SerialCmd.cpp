#include <SerialCmd.h>
#include <Global_vars.h>
#include <Voice.h>

void SerialCommands(){
    while(Serial.available()){
        String c = Serial.readString();
        if(c.startsWith("V")){
            int num = c.substring(1).toInt();
            pref.begin("volume",false);
            pref.putInt("vol",num);
            pref.end();
            change_volume(num);
        }
    }
}