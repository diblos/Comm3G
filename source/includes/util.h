/*
 * util.h
 *
 *  Created on: Jan 13, 2012
 *      Author: Justin
 */

#ifndef UTIL_H_
#define UTIL_H_

extern short 			get_hexchar(unsigned char hex);
extern unsigned char 	get_hexbyte(char hexchar);
extern int 				hexarray_to_hexstring(unsigned char* data, int data_len, char* hexstring, char spacing);
extern int 				hexstring_to_hexarray(char* hexstring, int hexstringlen, unsigned char* hexarray);
extern int 				set_short(short value, unsigned char* destbuf);
extern int 				set_ushort(unsigned short value, unsigned char* destbuf);
extern int 				set_int(int value, unsigned char* destbuf);
extern int 				set_uint(unsigned int value, unsigned char* destbuf);
extern short 			get_short(unsigned char* data);
extern unsigned short 	get_ushort(unsigned char* data);
extern int 				get_int(unsigned char* data);
extern unsigned int		get_uint(unsigned char *data);
extern unsigned char 	is_bcd(unsigned char* datain, int datainlen);
extern void				buf_and(unsigned char* buf1, unsigned char* buf2, int len, unsigned char* dest);
extern void				buf_or(unsigned char* buf1, unsigned char* buf2, int len, unsigned char* dest);
extern int				rtc_to_short_string(unsigned char* datetime7, char* displaybuf);//convert rtc datetime7 value to display string

//
// Framing
//
extern unsigned char framing_generate_crc16(unsigned char* DataIn, int DataInLength, unsigned char* Crc16);
extern unsigned char framing_verify_crc16(unsigned char* DataIn, int DataInLength, unsigned char* Crc16);
extern unsigned char framing_verify_crc16_atend(unsigned char* DataIn, int DataInLength);

#endif /* UTIL_H_ */
