#include <SerialCmd.h>
#include <Global_vars.h>
#include <Voice.h>
#include <CheckCode.h>
#include <pwm.h>
boolean test_reposition=false;
void SerialCommands(){
    while(Serial.available()){
        String c = Serial.readString();
        if(c.startsWith("V")){
            int num = c.substring(1).toInt();
            pref.begin("volume",false);
            pref.putInt("vol",num);
            pref.end();
            change_volume(num);
        }else if(c.startsWith("v")){
            int num = c.substring(1).toInt();
            play_voice(num);
        }else if(c.startsWith("test")){
            check_code("(2W3W1W2W4)");
        }else if(c.startsWith("repos")){
            Serial.println("reposition");
            test_reposition=true;
        }
    }
    if(test_reposition==true){
        pwm_oid_movement(1,1);
        test_reposition=false;
    }
}