/*
 * font.h
 *
 * Created: 2012-11-29 8:19:56 PM
 *  Author: Justin
 */ 


#ifndef FONT_H_
#define FONT_H_


//Default Fonts (From Display Driver)
extern const unsigned char FONT6x8[97][8];
extern const unsigned char FONT8x8[97][8];
extern const unsigned char FONT8x16[97][16];

//Plugin Fonts (font.c)
extern const unsigned char FontFixesys16[97][16];
extern const unsigned int FontMSSerif18[];
extern const unsigned int FontMSSerif24[];
extern const unsigned int FontMSSerif36[];

//
// Export
//
#define font_default8		((unsigned char*)&FONT6x8)
#define font_default8bold	((unsigned char*)&FONT8x8)
#define font_default16		((unsigned char*)&FONT8x16)
#define font_Fixesys16		((unsigned char*)&FontFixesys16)
#define font_Fixesys18		((unsigned char*)&FontFixesys18)
#define font_MsSerif18		((unsigned char*)&FontMSSerif18)
#define font_MsSerif24		((unsigned char*)&FontMSSerif24)
#define font_MsSerif36		((unsigned char*)&FontMSSerif36)

//
// Public
//
void font_GetStringSize(char* textString, const unsigned char *font_style, unsigned short* xsize, unsigned short* ysize);

#endif /* FONT_H_ */