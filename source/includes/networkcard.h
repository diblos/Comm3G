/*
 * networkcard.h
 *
 * Created: 2014-10-28 6:51:53 PM
 *  Author: Justin
 */ 


#ifndef NETWORKCARD_H_
#define NETWORKCARD_H_


#define NETWORKCARD_TYPE_WIFI_GS	1
#define NETWORKCARD_TYPE_WIFI_MN	2
#define NETWORKCARD_TYPE_3G_SC		3

//
// Error Codes
//
#define NETWORK_ER_OK				0
#define NETWORK_ER_FAILED			(-1)
									/*100 to 199 = state*/
#define NETWORK_ER_NOT_INITED		(-101)
#define NETWORK_ER_INPUT_OFF		(-102)/*Adatper is set to OFF*/
#define NETWORK_ER_IDLE				(-103)/*no task*/
#define NETWORK_ER_BUSY				(-104)/*Busy*/
#define NETWORK_ER_NOT_FOUND		(-105)/*SSID, Netowrk not found*/
#define NETWORK_ER_NET_ERROR		(-106)
#define NETWORK_ER_KEY_REQUIRED		-2

int networkcard_init(unsigned long baudrate);
int networkcard_task(void);


int networkcard_power_on(void);
int networkcard_power_off(void);
int networkcard_power_reset(void);

	//0- Not connect, not ready
	//1- connected, very poor signal
	//2 - connected, poor signal
	//3 - connected, fair signal
	//4 - connected, good signal
	//5 - connect, very good signal
int networkcard_get_signal_strength(void);
	
//debug
void netdebug_printbytes(char* msg, unsigned char* data, int datalen);
void netdebug_printstring(char* msg1, char* msg2);
void netdebug_print(char* msg);
void netdebug_printdec(char* msg1, int decimal);

#endif /* NETWORKCARD_H_ */