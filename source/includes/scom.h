/*
 * scom.h
 *
 * Created: 9/6/2016 10:57:31 AM
 *  Author: KM
 */ 

#ifndef SCOM_H_
#define SCOM_H_


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

typedef struct{
	char active[16];
	int Error;
	char dev[32];
	unsigned char module[16];
	int Signal;
	char APN[16];
	char userId[16];
	char Password[16];
}struct_pppStatus;


//Set PPP
//Status input: 1 turn ON, 0 turn OFF
//APN    input: if no need uid and pwd that did not need to change APN
//Input UID;
//Input Password;
extern int scom_set_ppp(int  status,char* apn,char* uid,char* pwd);

//this function return RSSI in positive, negative indicate error. For example. -97dmb returned as 97
extern int scom_get_signal_strength(void);

//return 0=Get status successful, status item in outPppStatus;
//return negative = error
extern int scom_get_status_flags(struct_pppStatus* outPppStatus);

#endif /* SCOM_H_ */