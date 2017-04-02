/*
 * theme.h
 *
 *  Created on: Mar 14, 2012
 *      Author: Justin
 */

#ifndef THEME_H_
#define THEME_H_

#include "gui.h"

//
// Display Measurement
//
#define THEME_STATUS_X				0
#define THEME_STATUS_Y				0
#define THEME_STATUS_WIDTH			LCD_WIDTH
#define THEME_STATUS_HEIGHT			24
#define THEME_WINDOW_X				0
#define THEME_WINDOW_Y				THEME_STATUS_HEIGHT
#define THEME_WINDOW_WIDTH			LCD_WIDTH
#define THEME_WINDOW_HEIGHT			(LCD_HEIGHT - THEME_STATUS_HEIGHT)

//
// COLOR
//
#define THEME_COLOR_FG				BLACK
#define THEME_COLOR_BG				(-1)
#define THEME_COLOR_STATUS_FG		BLACK

//
// FONT
//
#define THEME_FONT_TITLE			(unsigned char*)&FONT8x16
#define THEME_FONT_LIST				(unsigned char*)&FONT8x16
#define THEME_FONT_BUTTON			(unsigned char*)&FONT8x16


//
// Functions
//

extern void theme_draw_backdrop(unsigned short x, unsigned short y, unsigned short width, unsigned short height );
extern void theme_draw_backdrop_box(box displaybox);
extern box theme_estimate_msgbox_box(char* Title, char* message, char* input);
extern int theme_draw_msgbox_header(int x, int y);//return the next Y;
extern int theme_draw_msgbox_body(int x, int y, int height);
extern int theme_draw_msgbox_button(int x, int y, int width);
extern void theme_draw_basic_select(box displaybox);
extern void theme_draw_desc_select(box displaybox);
extern void theme_draw_basic_button(box displaybox);
extern void theme_draw_basic_title(box displaybox);
extern void theme_draw_desc_button(box displaybox);
extern void theme_draw_onff_icon(unsigned char value, box displaybox, unsigned char attr);
extern void theme_draw_checkbox(unsigned char value, box displaybox, unsigned char attr);
extern void theme_draw_volume(int minvalue, int maxvalue, int value, box displaybox, unsigned char attr);
extern void theme_draw_scroller(int minvalue, int maxvalue, int value, box displaybox, unsigned char attr);

#endif /* THEME_H_ */
