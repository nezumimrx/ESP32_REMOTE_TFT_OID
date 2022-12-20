#ifdef __has_include
    #if __has_include("lvgl.h")
        #ifndef LV_LVGL_H_INCLUDE_SIMPLE
            #define LV_LVGL_H_INCLUDE_SIMPLE
        #endif
    #endif
#endif

#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
    #include "lvgl.h"
#else
    #include "lvgl/lvgl.h"
#endif


#ifndef LV_ATTRIBUTE_MEM_ALIGN
#define LV_ATTRIBUTE_MEM_ALIGN
#endif

#ifndef LV_ATTRIBUTE_IMG_ARROW_LEFT_BW
#define LV_ATTRIBUTE_IMG_ARROW_LEFT_BW
#endif

const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST LV_ATTRIBUTE_IMG_ARROW_LEFT_BW uint8_t Arrow_left_bw_map[] = {
  0x24, 0xe3, 0xf2, 0xff, 	/*Color of index 0*///BRG
  0x33, 0x18, 0x09, 0xff,	/*Color of index 1*/

  0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 
  0xff, 0xff, 0xf0, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xfc, 
  0xff, 0xff, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xfc, 
  0xff, 0xff, 0xc0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xfc, 
  0xff, 0xff, 0x80, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xfc, 
  0xff, 0xff, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xfc, 
  0xff, 0xfe, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xfc, 
  0xff, 0xfc, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xfc, 
  0xff, 0xf8, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xfc, 
  0xff, 0xf0, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 
  0xff, 0xf0, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 
  0xff, 0xe0, 0x00, 0xbf, 0xff, 0xff, 0xff, 0xff, 0xfc, 
  0xff, 0xc0, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xfc, 
  0xff, 0x80, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xfc, 
  0xff, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xfc, 
  0xfe, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xfc, 
  0xfc, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xfc, 
  0xf8, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xfc, 
  0xf8, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xfc, 
  0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xfc, 
  0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xfc, 
  0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xfc, 
  0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xfc, 
  0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xfc, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xfc, 
  0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xfc, 
  0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xfc, 
  0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xfc, 
  0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xfc, 
  0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xfc, 
  0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xfc, 
  0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xfc, 
  0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xfc, 
  0xff, 0xc0, 0x00, 0x08, 0x00, 0x00, 0x00, 0x01, 0xfc, 
  0xff, 0xe0, 0x00, 0x7f, 0xf4, 0x00, 0x00, 0x01, 0xfc, 
  0xff, 0xf0, 0x00, 0x3f, 0xff, 0x80, 0x00, 0x00, 0xfc, 
  0xff, 0xfc, 0x00, 0x0f, 0xff, 0xf8, 0x00, 0x00, 0xfc, 
  0xff, 0xfe, 0x00, 0x07, 0xff, 0xfe, 0x00, 0x00, 0x7c, 
  0xff, 0xff, 0x00, 0x03, 0xff, 0xff, 0x80, 0x00, 0x7c, 
  0xff, 0xff, 0xc0, 0x03, 0xff, 0xff, 0xe0, 0x00, 0x3c, 
  0xff, 0xff, 0xe0, 0x01, 0xff, 0xff, 0xf0, 0x00, 0x3c, 
  0xff, 0xff, 0xf0, 0x01, 0xff, 0xff, 0xfc, 0x00, 0x1c, 
  0xff, 0xff, 0xf8, 0x03, 0xff, 0xff, 0xfe, 0x00, 0x1c, 
  0xff, 0xff, 0xfe, 0x03, 0xff, 0xff, 0xff, 0x00, 0x1c, 
  0xff, 0xff, 0xff, 0x07, 0xff, 0xff, 0xff, 0x80, 0x0c, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x0c, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x0c, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x04, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x04, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x04, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x04, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x04, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x04, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x04, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x04, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x04, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x04, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x04, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x84, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x04, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0c, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0c, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0c, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xbc, 
};

const lv_img_dsc_t Arrow_left_bw = {
  .header.cf = LV_IMG_CF_INDEXED_1BIT,
  .header.always_zero = 0,
  .header.reserved = 0,
  .header.w = 70,
  .header.h = 70,
  .data_size = 638,
  .data = Arrow_left_bw_map,
};
