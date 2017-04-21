/*
 * tps.h
 *
 * Created: 21/4/2017 2:56:23 PM
 *  Author: lenovo
 */ 


#ifndef TPS_H_
#define TPS_H_

typedef struct
{
    unsigned char second; // 0-59
    unsigned char minute; // 0-59
    unsigned char hour;   // 0-23
    unsigned char day;    // 1-31
    unsigned char month;  // 1-12
    unsigned char year;   // 0-99 (representing 2000-2099)
}
date_time_t;

#define Y2K 946684800

void getDeviceID(char* deviceIdHex);
unsigned int getTime(int timezone);
unsigned int getPrevHB(unsigned int currentDT,unsigned int startHB,unsigned int periodHB);
extern unsigned int getNextHB(unsigned int currentDT,unsigned int startHB,unsigned int periodHB);
extern void epoch_to_date_time(date_time_t* date_time,unsigned int epoch,int timezone);
unsigned int getNextCFG(unsigned int currentDT);

#endif /* TPS_H_ */