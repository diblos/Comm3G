/*
 * debug.h
 *
 * Created: 2012-03-28 8:35:54 PM
 *  Author: Justin
 */ 


#ifndef DEBUG_H_
#define DEBUG_H_


//
// Debug
//
#define DEBUG_PORT_DISPLAY		(1<<0)
#define DEBUG_PORT_SERIAL		(1<<1)

#define DEBUG_LVL_ERROR			1
#define DEBUG_LVL_APP			3
#define DEBUG_LVL_KERNEL		4
#define DEBUG_LVL_COMM			5 /*serial, usb, card, sam*/
#define DEBUG_LVL_CARD			5

extern void debug_set_level(unsigned char level);
extern void debug_set_port(unsigned int ports);
extern void debug_print(unsigned char level, char* msg);
extern void debug_printbytes(unsigned char level, char* msg, unsigned char* data, int data_len);



#endif /* DEBUG_H_ */