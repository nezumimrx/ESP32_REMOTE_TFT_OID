#include <Arduino.h>
#include <SPIFFS.h>//遇到的很奇怪的现象，这个SPIFFS库必须放在前面，否则File类会报错
#include <Global_vars.h>
#include <TFT_eSPI.h>
#include <TFT_blackbg.h>


/*
需要在下载的TFT_eSPI库中，找到User_Setup.h调整以下内容
注释掉 #define ILI9341_DRIVER 
启用 #define ST7735_DRIVER
启用 #define TFT_WIDTH  128
启用 #define TFT_HEIGHT 128
启用 #define ST7735_GREENTAB3
注释掉 #define TFT_CS   PIN_D8  // Chip select control pin D8
注释掉 #define TFT_DC   PIN_D3  // Data Command control pin
注释掉 #define TFT_RST  PIN_D4  // Reset pin (could connect to NodeMCU RST, see next line)
启用 #define TFT_MISO 19
启用 #define TFT_MOSI 23
启用 #define TFT_SCLK 18
启用 #define TFT_CS   15  // Chip select control pin
启用 #define TFT_DC    2  // Data Command control pin
启用 #define TFT_RST   4  // Reset pin (could connect to RST pin)
*/

unsigned char RowBmp128[2048];//
TFT_eSPI tft = TFT_eSPI(); // Invoke library
TFT_eSprite sprite = TFT_eSprite(&tft);
File myFile;
int previous_face_condition=0;//
int random_facial_type = 1;

void TFT_func_init()
{
  tft.begin(); // Initialise the display
  tft.fillScreen(TFT_BLACK);
  tft.setRotation(6);
  tft.drawXBitmap(2, 2, black_background, 128, 128, TFT_BLACK, TFT_BLACK);
  sprite.createSprite(128, 128);

  pinMode(25, OUTPUT);
  digitalWrite(25, HIGH);
  if (!SPIFFS.begin())
  {
    Serial.println("SPIFFS Mount Failed");
    return;
  }
}

void IRAM_ATTR DrawBmp(String name)
{
  myFile = SPIFFS.open(name, "r");
  // Serial.println(name);
  if (myFile)
  {

    while (myFile.available())
    {
      for (int i = 0; i < 0x3e; i++)
      {
        myFile.read();
      }
      for (int i = 0; i < 1408; i++)
      {
        RowBmp128[i] = myFile.read();
      }
      sprite.drawBitmap(0, 10, RowBmp128, 128, 88, TFT_WHITE, TFT_BLACK);
      sprite.pushSprite(0, 0);
      myFile.close();
    }
  }
  else
  {
    //Serial.print(F("error opening "));
    Serial.println(name);
  }
}

void TFT_usual(String switchbehavior,int start_index, int end_index)
{
  //example：2usual10 (1).bmp
  for (int i = start_index; i <= end_index; i++)
  {
    String filename = "";
    filename = "/2usual" + switchbehavior +" ("+ String(i) + ").bmp";
    DrawBmp(filename);
    vTaskDelay(5/portTICK_PERIOD_MS);
  }
}

void TFT_angry_blink(int start_index, int end_index)
{
  //example：2usual10 (1).bmp
  for (int i = start_index; i <= end_index; i++)
  {
    String filename = "";
    filename = "/4AngryBlink" + String(i) + ".bmp";
    DrawBmp(filename);
    vTaskDelay(5/portTICK_PERIOD_MS);
  }
}

void TFT_happy_blink(int start_index, int end_index)
{
  //example：2usual10 (1).bmp
  for (int i = start_index; i <= end_index; i++)
  {
    String filename = "";
    filename = "/4HappyBlink" + String(i) + ".bmp";
    DrawBmp(filename);
    vTaskDelay(5/portTICK_PERIOD_MS);
  }
}

void TFT_sad_blink(int start_index, int end_index)
{
  for (int i = start_index; i <= end_index; i++)
  {
    String filename = "";
    filename = "/4SadBlink" + String(i) + ".bmp";
    DrawBmp(filename);
    vTaskDelay(5/portTICK_PERIOD_MS);
  }
}

void TFT_usualExpression(){
    if(previous_face_condition!=0){
        sprite.drawXBitmap(2,2,black_background,128,128,TFT_BLACK,TFT_BLACK);
        sprite.pushSprite(0,0);
        if(previous_face_condition==3){//之前是生气表情
            TFT_angry_blink(1,2);//到眨眼合上眼
        }else if(previous_face_condition==4){//之前是开心
            TFT_happy_blink(1,4);//到眨眼合上眼
        }else if(previous_face_condition==5){//之前是难过
            TFT_sad_blink(1,4);//到眨眼合上眼
        }
        TFT_usual("01",1,7);
        previous_face_condition=0;
    }
    int temp_facial_switch_to = int(random(1,200));
  if(random_facial_type==1){
    DrawBmp("/2usual01 (7).bmp");
    if(temp_facial_switch_to==1){
      TFT_usual("10",1,7);
      TFT_usual("01",1,7);
      random_facial_type=1;
    }else if(temp_facial_switch_to==2){
      TFT_usual("10",1,7);
      TFT_usual("02",1,6);
      random_facial_type=2;
    }else if(temp_facial_switch_to==3){
      TFT_usual("10",1,7);
      TFT_usual("03",1,6);
      random_facial_type=3;
    }
  }else if(random_facial_type==2){
    DrawBmp("/2usual02 (6).bmp");
    if(temp_facial_switch_to==1){
      TFT_usual("20",1,6);
      TFT_usual("01",1,7);
      random_facial_type=1;
    }else if(temp_facial_switch_to==2){
      TFT_usual("20",1,6);
      TFT_usual("02",1,6);
      random_facial_type=2;
    }else if(temp_facial_switch_to==3){
      TFT_usual("20",1,6);
      TFT_usual("03",1,6);
      random_facial_type=3;
    }
  }else if(random_facial_type==3){
    DrawBmp("/2usual03 (6).bmp");
    if(temp_facial_switch_to==1){
      TFT_usual("30",1,6);
      TFT_usual("01",1,7);
      random_facial_type=1;
    }else if(temp_facial_switch_to==2){
      TFT_usual("30",1,6);
      TFT_usual("02",1,6);
      random_facial_type=2;
    }else if(temp_facial_switch_to==3){
      TFT_usual("30",1,6);
      TFT_usual("03",1,6);
      random_facial_type=3;
    }
  }

}

