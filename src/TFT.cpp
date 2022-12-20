#include <Arduino.h>
#include <Global_vars.h>
#include <TFT_eSPI.h>
#include <lvgl.h>

/*
需要在下载的TFT_eSPI库中，找到User_Setup.h调整以下内容
以下内容针对ILI9341驱动的显示器，
启用 #define ILI9341_DRIVER 
注释掉 #define TFT_CS   PIN_D8  // Chip select control pin D8
注释掉 #define TFT_DC   PIN_D3  // Data Command control pin
注释掉 #define TFT_RST  PIN_D4  // Reset pin (could connect to NodeMCU RST, see next line)
启用 #define TFT_MISO 19
启用 #define TFT_MOSI 23
启用 #define TFT_SCLK 18
启用 #define TFT_CS   15  // Chip select control pin
启用 #define TFT_DC    2  // Data Command control pin
启用 #define TFT_RST   4  // Reset pin (could connect to RST pin)
启用 #define TFT_RGB_ORDER TFT_BGR  // Colour order Blue-Green-Red//这个一定要打开，否则颜色是错的
*/

/* 
对于LVGL库则需要配置lv_conf.h
#define LV_COLOR_DEPTH 16
#define LV_COLOR_16_SWAP 0 //这个一定是0否则颜色是错的
修改图片.c文件的开头配色，按BGR顺序
0x24, 0xe3, 0xf2, 0xff, 	//BGR
#define LV_TICK_CUSTOM 1

*/
#define BMPWIDTH 240
#define BMPHEIGHT 320

lv_obj_t *animation_screen;
lv_obj_t *animation_page;
lv_obj_t *arrow_page;
lv_obj_t *left_pos_item;
lv_obj_t *middle_pos_item;
lv_obj_t *right_pos_item;

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
LV_IMG_DECLARE(Arrow_left_bw);
LV_IMG_DECLARE(Arrow_up_bw);
LV_IMG_DECLARE(Arrow_right_bw);
LV_IMG_DECLARE(loop2);
LV_IMG_DECLARE(loop3);
LV_IMG_DECLARE(loopend);


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

void create_anim_screen(){
  static lv_style_t no_border_style;
  lv_style_init(&no_border_style);
  lv_style_set_border_width(&no_border_style,0);
  animation_page = lv_obj_create(NULL);
  lv_obj_add_style(animation_page,&no_border_style,LV_PART_MAIN);
  lv_obj_set_size(animation_page,BMPWIDTH,BMPHEIGHT);
  lv_obj_set_style_bg_color(animation_page, lv_color_hex(0x000000), LV_PART_MAIN);//背景涂黑
  animation_screen=lv_animimg_create(animation_page); //创建一个animation screen
  lv_obj_center(animation_screen);
}

void create_arrow_screen(){
  static lv_style_t no_border_style;
  lv_style_init(&no_border_style);
  lv_style_set_border_width(&no_border_style,0);

  arrow_page = lv_obj_create(NULL);
  lv_obj_add_style(arrow_page,&no_border_style,LV_PART_MAIN);
  lv_obj_set_size(arrow_page,BMPWIDTH+10,BMPHEIGHT+10);
  lv_obj_set_style_bg_color(arrow_page, lv_color_hex(0x000000), LV_PART_MAIN);//背景涂黑
  lv_obj_center(arrow_page);
  //创建一个框框装代码图形
  static lv_style_t has_border_style;
  lv_style_init(&has_border_style);
  lv_style_set_border_width(&has_border_style,2);
  lv_style_set_border_color(&has_border_style,lv_color_hex(0xffffff));

  lv_obj_t *code_space = lv_obj_create(arrow_page);
  lv_obj_set_size(code_space,240,90);
  lv_obj_set_style_bg_color(code_space, lv_color_hex(0x091833), LV_PART_MAIN);//背景白色
  lv_obj_align(code_space,LV_ALIGN_CENTER,0,0);
  lv_obj_add_style(code_space,&has_border_style,LV_PART_MAIN);
  //
  left_pos_item=lv_img_create(arrow_page);
  middle_pos_item=lv_img_create(arrow_page);
  right_pos_item=lv_img_create(arrow_page);
  lv_obj_align(left_pos_item, LV_ALIGN_CENTER, -80, 0);
  lv_obj_align(middle_pos_item,LV_ALIGN_CENTER,0,0);
  lv_obj_align(right_pos_item,LV_ALIGN_CENTER,80,0);
  

  

}
void arrow_image_create(int left_image_type, int middle_image_type, int right_image_type){//最左侧是什么种类的图标？1-上2-右3-左，25-循环2次，26循环3次，27循环结束
  switch (left_image_type)
  {
  case 1:
    lv_img_set_src(left_pos_item,&Arrow_up_bw);
    break;
  case 2:
    lv_img_set_src(left_pos_item,&Arrow_right_bw);
    break;
  case 3:
    lv_img_set_src(left_pos_item,&Arrow_left_bw);
    break;
  case 25://循环2次，这个捆绑的是button_pressed,为么是25？参考button_pressed声明备注
    lv_img_set_src(left_pos_item,&loop2);
    break;
  case 26:
    lv_img_set_src(left_pos_item,&loop3);
    break;
  case 27:
    lv_img_set_src(left_pos_item,&loopend);
    break;
  default:
    lv_img_set_src(left_pos_item,NULL);
    break;
  }

  switch (middle_image_type)
  {
  case 1:
    lv_img_set_src(middle_pos_item,&Arrow_up_bw);
    break;
  case 2:
    lv_img_set_src(middle_pos_item,&Arrow_right_bw);
    break;
  case 3:
    lv_img_set_src(middle_pos_item,&Arrow_left_bw);
    break;
   case 25://循环2次，这个捆绑的是button_pressed,为么是25？参考button_pressed声明备注
    lv_img_set_src(middle_pos_item,&loop2);
    break;
  case 26:
    lv_img_set_src(middle_pos_item,&loop3);
    break;
  case 27:
    lv_img_set_src(middle_pos_item,&loopend);
    break;
  default:
    lv_img_set_src(middle_pos_item,NULL);
    break;
  }

  switch (right_image_type)
  {
  case 1:
    lv_img_set_src(right_pos_item,&Arrow_up_bw);
    break;
  case 2:
    lv_img_set_src(right_pos_item,&Arrow_right_bw);
    break;
  case 3:
    lv_img_set_src(right_pos_item,&Arrow_left_bw);
    break;
   case 25://循环2次，这个捆绑的是button_pressed,为么是25？参考button_pressed声明备注
    lv_img_set_src(right_pos_item,&loop2);
    break;
  case 26:
    lv_img_set_src(right_pos_item,&loop3);
    break;
  case 27:
    lv_img_set_src(right_pos_item,&loopend);
    break;
  default:
    lv_img_set_src(right_pos_item,NULL);
    break;
  }

}


void astronaut_img_create(void)
{
   lv_animimg_set_duration(animation_screen, 25*40);//25*40ms 
   lv_animimg_set_src(animation_screen, (lv_img_dsc_t **)astronaut_img_list, 25);
   lv_animimg_set_repeat_count(animation_screen, 1);
   lv_animimg_start(animation_screen);
   lv_scr_load(animation_page);
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
  

  create_anim_screen();
  astronaut_img_create();
  create_arrow_screen();
  
  lv_scr_load(animation_page);
}




void expression_img_create(void)
{
   lv_animimg_set_duration(animation_screen, 13*40);//13*40ms 
   lv_animimg_set_src(animation_screen, (lv_img_dsc_t **)expression_img_list, 13);
   lv_animimg_set_repeat_count(animation_screen, 1);
   lv_animimg_start(animation_screen);
}

void TFT_usualExpression(){
  if(previous_face_condition!=0){
    lv_scr_load(animation_page);
    previous_face_condition=0;
  }
  astronaut_img_create();
  //vTaskDelay(2050/portTICK_PERIOD_MS);
  //arrow_image_create(random(1,7),random(1,7),random(1,7));
  vTaskDelay(1000/portTICK_PERIOD_MS);


}

void TFT_drawArrow(){
  if(previous_face_condition!=2){
    lv_scr_load(arrow_page);
    
    if(commands_counter==0){
      face_condition=previous_face_condition;
    }else {
      face_condition=1;
      
      previous_face_condition=1;
    }
  }
  arrow_image_create(commands_array[2],commands_array[1],commands_array[0]);
  vTaskDelay(50/portTICK_PERIOD_MS);
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

