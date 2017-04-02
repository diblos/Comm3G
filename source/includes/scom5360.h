/*
 * scom5360.h
 *
 * Created: 2014-10-21 3:34:59 PM
 *  Author: Justin
 */ 


#ifndef SCOM5360_H_
#define SCOM5360_H_


#define SCOM_PWR							AVR32_PIN_PC05/*Out, AVR32_PIN_PA02*/
#define SCOM_DTR							AVR32_PIN_PA02/*Out, High=Sleep, Low=WakeUp*/
#define SCOM_RESET							AVR32_PIN_PX16/*Out, Default High, set to low for >50ms to reset module*/
#define SCOM_STATUS							AVR32_PIN_PX15/*Pwr Status: In, High=On, Low=Off*/
#define SCOM_LED							AVR32_PIN_PA20/*LED status: In, High=On, Low=Off*/

#define scom_usart                          (&AVR32_USART3)
#define SCOM_USART_RX_PIN                   AVR32_USART3_RXD_0_2_PIN    /*PX57*/
#define SCOM_USART_RX_FUNCTION              AVR32_USART3_RXD_0_2_FUNCTION
#define SCOM_USART_TX_PIN                   AVR32_USART3_TXD_0_2_PIN     /*PX54*/
#define SCOM_USART_TX_FUNCTION              AVR32_USART3_TXD_0_2_FUNCTION

//
// Extern
//
extern int scom_init(unsigned long baudrate);
extern int scom_task(void);

extern int scom_power_on(void);
extern int scom_power_off(void);
extern int scom_power_reset(void);/*hardware reset*/


extern char* scom_get_status_label(void);


//Obtain RSSI
//negative = error code
//0- Not connect, not ready
//1- connected, very poor signal
//2 - connected, poor signal
//3 - connected, fair signal
//4 - connected, good signal
//5 - connect, very good signal
	
// RSSI	 Range		mapindex
//	<= -99			1
//	100		87		2
//	86		75		3
//	74		63		4
//	62		51		5

//this function return RSSI in positive, negative indicate error. For example. -97dmb returned as 97
extern int scom_get_signal_strength(void);


#define SCOM_STFLAG_PW_OFF		0x00000000
#define SCOM_STFALG_PW_CHK		0x00000040
#define SCOM_STFLAG_PW_ON		0x00000080
#define SCOM_STFLAG_SIM_READY	0x00000020
#define SCOM_STFLAG_NW_REGISTED	0x00000010
#define SCOM_STFLAG_IS_3G		0x00000008
unsigned int scom_get_status_flags(void);

//
// Data Service
//

//Mandatory function to activate the data channel service before any socket function allowed
//recommended to stop this service once transmission completed for power efficiency
//APN shall be ensure set and correct before calling this function
//IP will be returned by this function
int scom_start_data_service(char** outOptIpAddr);

//Stop data service
int scom_stop_data_service(void);

//make a TCP connection over cellurar network
int scom_tcp_connect(char* ip, char* port, int* outConnectionId);

//disconnect socket connection
int scom_tcp_disconnect(int connectionId);

//disconnect all opened socket
int scom_tcp_disconnect_all(void);

//Write TCP data
int scom_tcp_write(int connectionId, unsigned char* data, int datalen);

//Read TCP Data
//the data in receive buffer stay forever until tcp flush function is call
int scom_tcp_read(int* connectionIdPtr, unsigned char** data, int timeoutms);

//flush tcp receive buffer
//in: provide length to specify the data length from buffer should be flush. set to 0 to flush all
int scom_tcp_flush(int ConnectionId, int flushlen);

#endif /* SCOM5360_H_ */