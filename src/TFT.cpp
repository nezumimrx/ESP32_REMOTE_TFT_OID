#include <Arduino.h>
#include <Global_vars.h>
#include <TFT_eSPI.h>
#include <lvgl.h>

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
#define BMPWIDTH 240
#define BMPHEIGHT 320

lv_obj_t *animation_screen;
int test_counter=0;
int test_plus=-1;

TFT_eSPI tft = TFT_eSPI(BMPWIDTH,BMPHEIGHT); // Invoke library
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[ BMPWIDTH * 10 ];
int previous_face_condition=0;//
int random_facial_type = 1;
LV_IMG_DECLARE(test1);
LV_IMG_DECLARE(test2);
LV_IMG_DECLARE(test3);
LV_IMG_DECLARE(test4);
LV_IMG_DECLARE(test5);
LV_IMG_DECLARE(test6);
LV_IMG_DECLARE(test7);
LV_IMG_DECLARE(test8);
LV_IMG_DECLARE(test9);
LV_IMG_DECLARE(test10);
LV_IMG_DECLARE(test11);
LV_IMG_DECLARE(test12);
LV_IMG_DECLARE(test13);
const lv_img_dsc_t *astronaut_img_list[25] = {&test1,&test2,&test3,&test4,&test5,&test6,&test7,&test8,&test9,&test10,&test11,&test12,&test13,
&test12,&test11,&test10,&test9,&test8,&test7,&test6,&test5,&test4,&test3,&test2,&test1,
};
const lv_img_dsc_t *expression_img_list[13] = {&test1,&test1,&test1,&test1,&test1,&test1,&test1,&test1,&test1,&test1,&test1,&test1,&test1,
};

lv_img_dsc_t *animation_img_list[30];

void my_disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p )
{
    uint32_t w = ( area->x2 - area->x1 + 1 );
    uint32_t h = ( area->y2 - area->y1 + 1 );
    tft.startWrite();
    tft.setAddrWindow( area->x1, area->y1, w, h );
    tft.pushColors( ( uint16_t * )&color_p->full, w * h, true );
    tft.endWrite();
    lv_disp_flush_ready( disp );
}
void TFT_func_init()
{
  tft.begin(); // Initialise the display
  lv_init();
  tft.setRotation(0);
  lv_disp_draw_buf_init( &draw_buf, buf, NULL, BMPWIDTH * 10 );
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init( &disp_drv );
  disp_drv.hor_res = BMPWIDTH;
  disp_drv.ver_res = BMPHEIGHT;
  disp_drv.flush_cb = my_disp_flush; //这里的my_disp_flush是关键，目前是16色，要改成单色
  disp_drv.draw_buf = &draw_buf;
  lv_disp_t * disp;
  disp = lv_disp_drv_register(&disp_drv); /*Register the driver and save the created display objects*/
  //涂上黑色背景，注意是16色
  lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x000000), LV_PART_MAIN);//背景涂黑
  animation_screen=lv_animimg_create(lv_scr_act()); //创建一个animation screen
  lv_obj_center(animation_screen);
}


void astronaut_img_create(void)
{
   lv_animimg_set_duration(animation_screen, 25*40);//25*40ms 
   lv_animimg_set_src(animation_screen, (lv_img_dsc_t **)astronaut_img_list, 25);
   lv_animimg_set_repeat_count(animation_screen, 1);
   lv_animimg_start(animation_screen);
}

void expression_img_create(void)
{
   lv_animimg_set_duration(animation_screen, 13*40);//13*40ms 
   lv_animimg_set_src(animation_screen, (lv_img_dsc_t **)expression_img_list, 13);
   lv_animimg_set_repeat_count(animation_screen, 1);
   lv_animimg_start(animation_screen);
}

void TFT_usualExpression(){
  astronaut_img_create();
  
  vTaskDelay(1050/portTICK_PERIOD_MS);
  //static lv_style_t style_move;
  //lv_style_init(&style_move);
  //lv_style_set_translate_x(&style_move, -20);
  //lv_obj_add_style(animation_screen, &style_move, LV_STATE_DEFAULT);
  //expression_img_create();
  //vTaskDelay(2000/portTICK_PERIOD_MS);
}
// void TFT_usual(String switchbehavior,int start_index, int end_index)
// {
//   //example：2usual10 (1).bmp
//   for (int i = start_index; i <= end_index; i++)
//   {
//     String filename = "";
//     filename = "/2usual" + switchbehavior +" ("+ String(i) + ").bmp";
//     DrawBmp(filename);
//     vTaskDelay(5/portTICK_PERIOD_MS);
//   }
// }

// void TFT_angry_blink(int start_index, int end_index)
// {
//   //example：2usual10 (1).bmp
//   for (int i = start_index; i <= end_index; i++)
//   {
//     String filename = "";
//     filename = "/4AngryBlink" + String(i) + ".bmp";
//     DrawBmp(filename);
//     vTaskDelay(5/portTICK_PERIOD_MS);
//   }
// }

// void TFT_happy_blink(int start_index, int end_index)
// {
//   //example：2usual10 (1).bmp
//   for (int i = start_index; i <= end_index; i++)
//   {
//     String filename = "";
//     filename = "/4HappyBlink" + String(i) + ".bmp";
//     DrawBmp(filename);
//     vTaskDelay(5/portTICK_PERIOD_MS);
//   }
// }

// void TFT_sad_blink(int start_index, int end_index)
// {
//   for (int i = start_index; i <= end_index; i++)
//   {
//     String filename = "";
//     filename = "/4SadBlink" + String(i) + ".bmp";
//     DrawBmp(filename);
//     vTaskDelay(5/portTICK_PERIOD_MS);
//   }
// }

// void TFT_usualExpression(){
//   //DrawBmp("/001.bmp");
//   //DrowBmp("/1.bmp");
//   for(int i=1;i<=13;i++){
//     String filename="";
//     filename="/"+String(i)+".bmp";
//     DrowBmp(filename);
//     vTaskDelay(5/portTICK_PERIOD_MS);
//   }
//   //tft.setRotation(4);
//   //tft.setRotation(6);
//     /*if(previous_face_condition!=0){
//         sprite.drawXBitmap(2,2,black_background,128,128,TFT_BLACK,TFT_BLACK);
//         sprite.pushSprite(0,0);
//         if(previous_face_condition==3){//之前是生气表情
//             TFT_angry_blink(1,2);//到眨眼合上眼
//         }else if(previous_face_condition==4){//之前是开心
//             TFT_happy_blink(1,4);//到眨眼合上眼
//         }else if(previous_face_condition==5){//之前是难过
//             TFT_sad_blink(1,4);//到眨眼合上眼
//         }
//         TFT_usual("01",1,7);
//         previous_face_condition=0;
//     }
//     int temp_facial_switch_to = int(random(1,200));
//   if(random_facial_type==1){
//     DrawBmp("/2usual01 (7).bmp");
//     if(temp_facial_switch_to==1){
//       TFT_usual("10",1,7);
//       TFT_usual("01",1,7);
//       random_facial_type=1;
//     }else if(temp_facial_switch_to==2){
//       TFT_usual("10",1,7);
//       TFT_usual("02",1,6);
//       random_facial_type=2;
//     }else if(temp_facial_switch_to==3){
//       TFT_usual("10",1,7);
//       TFT_usual("03",1,6);
//       random_facial_type=3;
//     }
//   }else if(random_facial_type==2){
//     DrawBmp("/2usual02 (6).bmp");
//     if(temp_facial_switch_to==1){
//       TFT_usual("20",1,6);
//       TFT_usual("01",1,7);
//       random_facial_type=1;
//     }else if(temp_facial_switch_to==2){
//       TFT_usual("20",1,6);
//       TFT_usual("02",1,6);
//       random_facial_type=2;
//     }else if(temp_facial_switch_to==3){
//       TFT_usual("20",1,6);
//       TFT_usual("03",1,6);
//       random_facial_type=3;
//     }
//   }else if(random_facial_type==3){
//     DrawBmp("/2usual03 (6).bmp");
//     if(temp_facial_switch_to==1){
//       TFT_usual("30",1,6);
//       TFT_usual("01",1,7);
//       random_facial_type=1;
//     }else if(temp_facial_switch_to==2){
//       TFT_usual("30",1,6);
//       TFT_usual("02",1,6);
//       random_facial_type=2;
//     }else if(temp_facial_switch_to==3){
//       TFT_usual("30",1,6);
//       TFT_usual("03",1,6);
//       random_facial_type=3;
//     }
//   }*/

// }

