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

#ifndef LV_ATTRIBUTE_IMG_LOOP2
#define LV_ATTRIBUTE_IMG_LOOP2
#endif

const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST LV_ATTRIBUTE_IMG_LOOP2 uint8_t loop2_map[] = {
  0x91, 0xbf, 0x6b, 0xff, 	/*Color of index 0*/
  0x33, 0x18, 0x09, 0xff,	/*Color of index 1*/

  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 
  0xff, 0xff, 0xff, 0xd0, 0x00, 0xff, 0xff, 0xff, 0xfc, 
  0xff, 0xff, 0xfe, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xfc, 
  0xff, 0xff, 0xf8, 0x00, 0x00, 0x03, 0xff, 0xff, 0xfc, 
  0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xfc, 
  0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xfc, 
  0xff, 0xfe, 0x00, 0x01, 0x50, 0x00, 0x0f, 0xff, 0xfc, 
  0xff, 0xfc, 0x00, 0x3f, 0xff, 0x80, 0x07, 0xff, 0xfc, 
  0xff, 0xf0, 0x01, 0xff, 0xff, 0xe0, 0x03, 0xff, 0xfc, 
  0xff, 0xe0, 0x07, 0xff, 0xff, 0xfc, 0x00, 0xff, 0xfc, 
  0xff, 0xc0, 0x1f, 0xff, 0xff, 0xfe, 0x00, 0x7f, 0xfc, 
  0xff, 0x80, 0x3f, 0xff, 0xff, 0xff, 0x80, 0x3f, 0xfc, 
  0xff, 0x00, 0xff, 0xfc, 0x0b, 0xff, 0xc0, 0x3f, 0xfc, 
  0xfe, 0x01, 0xff, 0xe0, 0x00, 0xff, 0xe0, 0x1f, 0xfc, 
  0xfe, 0x03, 0xff, 0x80, 0x00, 0x3f, 0xf8, 0x0f, 0xfc, 
  0xfc, 0x07, 0xfe, 0x00, 0x00, 0x1f, 0xf8, 0x07, 0xfc, 
  0xf8, 0x0f, 0xfc, 0x00, 0x00, 0x0f, 0xfc, 0x07, 0xfc, 
  0xf8, 0x0f, 0xfc, 0x00, 0x00, 0x07, 0xfe, 0x03, 0xfc, 
  0xf0, 0x1f, 0xf8, 0x03, 0xf0, 0x07, 0xff, 0x01, 0xfc, 
  0xf0, 0x3f, 0xf8, 0x07, 0xf8, 0x03, 0xff, 0x01, 0xfc, 
  0xe0, 0x3f, 0xf0, 0x0f, 0xfc, 0x03, 0xff, 0x81, 0xfc, 
  0xe0, 0x7f, 0xf0, 0x1f, 0xfe, 0x03, 0xff, 0x80, 0xfc, 
  0xe0, 0x7f, 0xf0, 0x1f, 0xfe, 0x03, 0xff, 0xc0, 0xfc, 
  0xc0, 0xff, 0xf0, 0x3f, 0xfe, 0x03, 0xff, 0xc0, 0x7c, 
  0xc0, 0xff, 0xf0, 0x3f, 0xfe, 0x03, 0xff, 0xe0, 0x7c, 
  0xc1, 0xff, 0xf0, 0x7f, 0xfe, 0x03, 0xff, 0xe0, 0x7c, 
  0x81, 0xff, 0xf8, 0xff, 0xfc, 0x03, 0xff, 0xf0, 0x7c, 
  0x81, 0xff, 0xff, 0xff, 0xfc, 0x07, 0xff, 0xf0, 0x3c, 
  0x81, 0xff, 0xff, 0xff, 0xf8, 0x07, 0xff, 0xf0, 0x3c, 
  0x81, 0xff, 0xff, 0xff, 0xf8, 0x07, 0xff, 0xf0, 0x3c, 
  0x83, 0xff, 0xff, 0xff, 0xf0, 0x0f, 0xff, 0xf0, 0x3c, 
  0x81, 0xff, 0xff, 0xff, 0xc0, 0x1f, 0xff, 0xf0, 0x3c, 
  0x83, 0xff, 0xff, 0xff, 0x80, 0x1f, 0xff, 0xf8, 0x3c, 
  0x83, 0xff, 0xff, 0xff, 0x00, 0x3f, 0xff, 0xf0, 0x3c, 
  0x83, 0xff, 0xff, 0xfe, 0x00, 0xff, 0xff, 0xf8, 0x3c, 
  0x83, 0xff, 0xff, 0xfc, 0x00, 0xff, 0xff, 0xf0, 0x3c, 
  0x81, 0xff, 0xff, 0xf0, 0x03, 0xff, 0xff, 0xf0, 0x3c, 
  0x81, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xf0, 0x3c, 
  0x81, 0xff, 0xff, 0xc0, 0x0f, 0xff, 0xff, 0xf0, 0x3c, 
  0x81, 0xff, 0xff, 0x80, 0x3f, 0xff, 0xff, 0xf0, 0x7c, 
  0xc1, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xf0, 0xe0, 0x7c, 
  0xc0, 0xff, 0xfe, 0x00, 0xff, 0xff, 0xf0, 0x60, 0x7c, 
  0xc0, 0xff, 0xfc, 0x01, 0xff, 0xff, 0xe0, 0x60, 0x7c, 
  0xc0, 0xff, 0xf8, 0x03, 0xff, 0xff, 0xf0, 0x40, 0xfc, 
  0xe0, 0x7f, 0xf8, 0x07, 0xff, 0xff, 0xf0, 0x00, 0xfc, 
  0xe0, 0x7f, 0xf0, 0x02, 0xaa, 0xaf, 0xf0, 0x00, 0xfc, 
  0xe0, 0x3f, 0xf0, 0x00, 0x00, 0x03, 0xf8, 0x01, 0xcc, 
  0xf0, 0x3f, 0xf0, 0x00, 0x00, 0x01, 0xf8, 0x00, 0x04, 
  0xf0, 0x1f, 0xe0, 0x00, 0x00, 0x01, 0xf8, 0x00, 0x00, 
  0xf8, 0x0f, 0xf0, 0x00, 0x00, 0x01, 0xf8, 0x00, 0x00, 
  0xf8, 0x0f, 0xf8, 0x00, 0x00, 0x03, 0xfc, 0x00, 0x04, 
  0xfc, 0x07, 0xfc, 0x00, 0x00, 0x07, 0xfc, 0x00, 0x04, 
  0xfe, 0x03, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x3c, 
  0xfe, 0x01, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x01, 0xfc, 
  0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0f, 0xfc, 
  0xff, 0x80, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 
  0xff, 0xc0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 
  0xff, 0xe0, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 
  0xff, 0xf0, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 
  0xff, 0xf8, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xfc, 
  0xff, 0xfe, 0x00, 0x02, 0xbf, 0xff, 0xff, 0xff, 0xfc, 
  0xff, 0xff, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xfc, 
  0xff, 0xff, 0xc0, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xfc, 
  0xff, 0xff, 0xf0, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xfc, 
  0xff, 0xff, 0xfe, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xfc, 
  0xff, 0xff, 0xff, 0xc0, 0x1f, 0xff, 0xff, 0xff, 0xfc, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 
};

const lv_img_dsc_t loop2 = {
  .header.cf = LV_IMG_CF_INDEXED_1BIT,
  .header.always_zero = 0,
  .header.reserved = 0,
  .header.w = 70,
  .header.h = 70,
  .data_size = 638,
  .data = loop2_map,
};
