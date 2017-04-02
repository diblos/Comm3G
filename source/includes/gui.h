/*
 * gui.h
 *
 *  Created on: Mar 7, 2012
 *      Author: Justin
 */

#ifndef GUI_H_
#define GUI_H_
#include "color.h"
#include "h2core.h"
#include "h2framework.h"

//
// LCD INFORMATION
//
#define LCD_WIDTH					320
#define LCD_HEIGHT					240

//
// COLOR CODE
//
#define GUI_SCHEME_DEFAULT			0x00
#define GUI_SCHEME_DESKTOP			0x01

#define GUI_COLOR_STATUS_BAR		BLACK
#define GUI_COLOR_STATUS_FONT		GRAY_LEV(20)
#define GUI_COLOR_MAIN_AREA			GRAY_LEV(5)

//
// STANDARD LENGTH
//
#define GUI_HEIGHT_STATUS_BAR		24
#define GUI_WIDTH_ICON				120
#define GUI_HEIGHT_ICON				120

//
// Display Attribute
//
#define ATTR_ALIGN_LEFT				0x0001
#define ATTR_ALIGN_RIGHT			0x0002
#define ATTR_ALIGN_CENTER			0x0004
#define ATTR_ALIGN_TOP				0x0008
#define ATTR_ALIGN_BOTTOM			0x0010
#define ATTR_ALIGN_MIDDLE			0x0020
#define ATTR_INVERT_CLR				0x0040
#define ATTR_PASSWORD				0x8000
#define ATTR_DECIMAL2				0x4000
#define ATTR_IP						0x2000

//
// GUI TYPE
//
#define GT_TITLE				0x01
#define GT_LABEL				0x01
#define GT_LIST					0x02
#define GT_OPTIONBOX			0x03
#define GT_CHECKBOX				0x04
#define GT_ONOFF				0x05
#define GT_BUTTON				0x06
#define GT_VOLUME				0x07 /*3 bytes value data, first byte MIN, second byte MAX, third byte NOW*/
#define GT_2DIGIT				0x12 /*3 bytes value data, first byte MIN, second byte MAX, third byte NOW*/

//
// MSGBOX
//
#define MSGBOX_WIDTH				280
#define MSGBOX_HEIGHT				200
#define MSGBOX_HEAD_HEIGHT			24
#define MSGBOX_MARGIN				10
#define MSGBOX_INPUT_HEIGHT			36
#define MSGBOX_BUTTON_HEIGHT		36
#define MSGBOX_COMPONENT_SPACING	10
#define MSGBOX_FOOT_HEIGH			10

#define MSGBOX_T_OK				0x01
#define MSGBOX_T_YESNO			0x02
#define MSGBOX_T_WAIT			0x03

#define MSGBOX_T_INPUT			0x80 /*first bit use for indicate input*/
#define MSGBOX_T_INPUT_STR		0x81 /*Not Supported -> The same with MSGBOX_T_INPUT*/
#define MSGBOX_T_INPUT_AMT		0x82 /*To use this, void* input must specify the decimal point, for example "0.00" for 2 points*/
#define MSGBOX_T_INPUT_DECIMAL	0x84 
#define MSGBOX_T_INPUT_PW		0x85
#define MSGBOX_T_INPUT_MYCP		0x86//MALAYSIA CAR PLATE
#define MSGBOX_T_INPUT_ONLY		0x40

#define MSGBOX_R_OK				0x01
#define MSGBOX_R_YES			0x01
#define MSGBOX_R_NO				0x00
#define MSGBOX_R_CANCEL			0x00
#define MSGBOX_R_BACK			0x00

//
// LIST EVENTS
//
#define LIST_EVENT_NULL			0
#define LIST_EVENT_SELECT		1
#define LIST_EVENT_CHANGE		2
#define LIST_EVENT_LONG_SELECT	3

#define LIST_TYPE_BASIC1		1
#define LIST_TYPE_BASIC2		2
#define LIST_TYPE_DESC1			3		
/*#define LIST_TYPE_DESC2			4 Not In Use*/
#define LIST_TYPE_BIGMENU1		5
//
// Events
//
#define ON_NO_REQ				0
#define ON_SELECT				1
//
// Public Structure
//
typedef struct{/*Text Box*/
	int id;
	unsigned short x;
	unsigned short y;
	unsigned short width;
	unsigned short height;
	unsigned short attr;
	unsigned char multiline;
}textbox;

#ifdef H2FRAMEWORK_FOR_OCEAN
#define LISTVIEW_MAX_STRING_LEN		24
#else
#define LISTVIEW_MAX_STRING_LEN		48
#endif

typedef struct{/*List View Item*/
	int id;
	unsigned char icon;
	unsigned char type;
	char label[LISTVIEW_MAX_STRING_LEN];
	char value[LISTVIEW_MAX_STRING_LEN];
	unsigned short valuetype;
	unsigned char isUpdated;
}listviewitem;


typedef struct{/*List View*/
	char* nameptr;
	unsigned short x1;
	unsigned short y1;
	unsigned short witdh;
	unsigned short height;
	unsigned char type;
	listviewitem* items;
	unsigned short maxitems;
	unsigned short count;/*current used items*/
	int startindex;
	int selectionindex;
	unsigned short maxrow;
	unsigned short bgcolor;
	unsigned short fgcolor;
	unsigned short* backdropmap;
	unsigned short selection_fgcolor;
	unsigned short selection_bgcolor;
	unsigned short* selection_backdropmap;
	unsigned short animatedelay;
}listview;

typedef struct{/*message box*/
	int x1;
	int y1;
	int witdh;
	int height;
	unsigned short tbgcolor;/*title background color*/
	unsigned short tfgcolor;/*title fore color*/
	unsigned short bgcolor;
	unsigned short fgcolor;
}msgboxstyle;

typedef struct{
	int x;
	int y;
	int width;
	int height;
	int margin;
}box;

//
// Public Functions
//
// Draw
extern int gui_draw_textbox(char *text, const unsigned char *font_style, unsigned short fcolor, int bcolor, box textbox, unsigned short attr);
extern int gui_draw_textbox_multiline(char *text, const unsigned char *font_style, unsigned short fcolor, int bcolor, box textbox, unsigned short attr);
extern int gui_draw_descbox(char *text1, char* text2, unsigned short fcolor, int bcolor, box textbox, unsigned short attr);
extern int gui_draw_bigmenubox(char *text1, char* text2, unsigned short fcolor, int bcolor, box textbox, unsigned short attr);
extern void gui_draw_fillbox(box displaybox, unsigned short color);
extern void gui_draw_status_bar();
extern void gui_status_bar_set_scheme(unsigned char schemeId);

// List View
extern void gui_listview_initilize_component(listview* lv, unsigned char type, unsigned short x, unsigned short y, unsigned short width, unsigned short height, char* nameptr, listviewitem* items, unsigned short maxitemscount);
extern void gui_listview_set_color_scheme(listview* lv, unsigned short colorschemecode);
extern int gui_listview_additem(listview* lv, unsigned char icon, unsigned char type, char* label, void* value);
extern int gui_listview_updateitem(listview* lv, int Index, unsigned char icon, unsigned char type, char* label, char* value);
extern int gui_listview_updateitem_value(listview* lv, int Index, char* value);
extern void gui_listview_show(listview* lv);
extern void gui_listview_select_previous();
extern void gui_listview_select_next();
extern int gui_listview_process_key_event(unsigned char event, unsigned char key);
extern int gui_listview_get_selected_index();

extern int gui_listview_get_item_value(int Index, void** value);
extern unsigned char gui_listview_get_label(int index, char** label);

//
// Progress Bar
//

typedef struct{
	unsigned char type;
	unsigned char value;
	unsigned short x;
	unsigned short y;
	unsigned short color1;
	unsigned short color2;
}progressbar;

extern void gui_draw_progress(progressbar* bar, unsigned char move);


// Msgbox
extern void msgbox_show(unsigned short ref_act_code, char* title, char* message, unsigned char msgboxtype, void* input32/*Input buffer must be 32 bytes*/);
extern unsigned char msgbox_showdialog(char* title, char* message, unsigned char msgboxtype, void* input32/*Input buffer must be 32 bytes*/);
extern unsigned char msgbox_showinputbox(unsigned short x, unsigned short y, unsigned char InputType, void* input32);

extern void msgbox_close(void);//close exist message box if any

//ICON
extern void gui_draw_icon(unsigned char* icommap, unsigned short x, unsigned short y, unsigned short color, unsigned short bgcolor);


// Status Bar
extern void gui_statusbar_update(void);

//GUI Debug
extern void gui_debug_print(char* msg);
extern void gui_debug_printbytes(char* msg, unsigned char* data, int datalen);

//GUI Utils
extern void gutil_set_string(char* source, char* dest, int maxdestsize);
extern int gutil_get_x_for_center_fit(int boxwidth);
extern int gutil_get_y_for_middle_fit(int boxheight);
extern int gutil_right_fit(int stringlen, int fontwidth);
extern int gutil_center_fit(int stringlen, int fontwidth);
extern int gutil_middle_fit(int stringheight);
extern int gutil_convert_bool_to_string(unsigned char boolean, char* destbuf);
extern int gutil_convert_udecimal_to_string(unsigned long data, char* destbuf);
extern int gutil_convert_decimal_to_string(long data, char* destbuf);
extern int gutil_get_string_in_center_box(box outerbox, box innerbox);


#endif /* GUI_H_ */
