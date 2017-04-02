/*
 * color.h
 *
 * Created: 2012-03-23 7:27:42 PM
 *  Author: Justin
 */ 


#ifndef COLOR_H_
#define COLOR_H_

/* http://en.wikipedia.org/wiki/Web_colors */

#define BLACK	0x0000
#define BLUE	0x001F
#define RED		0xF800
#define LIME	0x07E0
#define WHITE	0xFFFF
#define ORANGE	0xFA80 /*31 20 00*/
#define YELLOW	0xFFE0
#define MAROON	0x7800
#define GREEN	0x0200 /*0x01E0*/
#define AQUA	0x07FF
#define NAVY	0x000F
#define MAGENTA	0xF81F
#define PURPLE	0x780F
#define PURPLEDIM	0x79EF
#define PURPLELIGHT	0xCAF7
#define GRAY	0x7BCF /*GRAY_LEV(15)*/
#define SILVER	0xB596 /*GRAY_LEV(22)*/
#define GOLD	0xFB40

#define TRANSPARENT (-1)

#define BLUE_LEV( level)  ((level)&BLUE)											// level is in [0; 31]
#define GREEN_LEV(level)  ((((level)*2)<<5)&LIME)									// level is in [0; 31]
#define RED_LEV(  level)  (((level)<<(5+6))&RED)									// level is in [0; 31]
#define GRAY_LEV( level)  ( BLUE_LEV(level) | GREEN_LEV(level) | RED_LEV(level) )	// level is in [0; 31]
#define RGB_LEV(r,g,b)	( RED_LEV(r) | GREEN_LEV(g) | BLUE_LEV(b) )					// level is in [0; 31]


#endif /* COLOR_H_ */