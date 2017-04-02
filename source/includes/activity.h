/*
 * activity.h
 *
 *  Created on: Jan 12, 2012
 *      Author: Justin
 */

#ifndef ACTIVITY_H_
#define ACTIVITY_H_

//
// Global Definitions
//
#define ACTIVITY_NULL			0x0000
#define ACTIVITY_CURRENT_ID		0x0000

#define ACTCODE_START			0x8001
#define ACTCODE_PAUSE			0x8002
#define ACTCODE_RESUME			0x8003
#define ACTCODE_STOP			0x8004

#define ACTCODE_KEY_EVENT 		0x8011 
#define ACTCODE_MSGBOX_EVENT	0x8012
#define ACTCODE_CLOCK_EVENT		0x8013

//
// System Activity ID (0x7000 to 0xFFFF) (Reserved)
//
#define ACTIVITY_FW_ABOUT			0x7001
#define ACTIVITY_MSGBOX				0x7002
#define ACTIVITY_FW_SETTING			0x7003
#define ACTIVITY_FW_DATETIME		0x7004
#define ACTIVITY_FW_WIFI			0x7005 /*WIFI Setting*/
#define ACTIVITY_FW_WIFI_INFO		0x7006
#define ACTIVITY_FW_WIFI_NETWORK	0x7007
#define ACTIVITY_FW_WIFI_ADVANCE	0x7008
#define ACTIVITY_FW_DEV_OPTION		0x7009

//
// Public Functions
//
extern int activity_register(unsigned short activity_id, void* activity_fuction_pointer);
extern void activity_start(short activity_id, short activity_code, unsigned char* data, int data_len);
extern void activity_exit(short activity_id);
extern void activity_exit_and_start(short activity_id, short activity_code, unsigned char* data, short data_len);
extern short activity_get_current_act_id();
extern void activity_set_clock(unsigned long periodms);

#endif /* ACTIVITY_H_ */
