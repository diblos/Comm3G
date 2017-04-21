/*
 * tps.c
 *
 * Created: 21/4/2017 2:55:14 PM
 *  Author: lenovo
 */ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "debug.h"
#include "tps.h"
#include "h2core.h"
#include "pcd.h"

#define HOUR 3600;

int isFirst = 1;
int FirstTime = 0;

void getDeviceID(char* deviceIdHex){
	char* getid[50];
	int idlen =0;
	
	pcd_get_info(PCD_EEA_PRODUCT_SN, getid, idlen);// PCD Serial Number
			
	for(int i = 0; i<idlen; i++){	
		sprintf(&deviceIdHex[strlen(deviceIdHex)], "%c", getid[i]);
	}
	memset(deviceIdHex,0,sizeof(deviceIdHex));
	sprintf(deviceIdHex,"%s", getid);
	debug_printstring(4, "DeviceID=", deviceIdHex);
}

void getEventCode(char* eventcode,int ac,int batt,int tamper){
	/*
	900 = No Event
	901 = No Power
	902 = No Battery
	903 = Low Battery
	904 = Tampered Case
	*/
	sprintf(eventcode,"900");
	if(ac==0){sprintf(eventcode,"901");};
	if(batt==0){sprintf(eventcode,"902");};
	if(tamper==0){sprintf(eventcode,"904");};
}

static unsigned short days[4][12] =
{
    {   0,  31,  60,  91, 121, 152, 182, 213, 244, 274, 305, 335},
    { 366, 397, 425, 456, 486, 517, 547, 578, 609, 639, 670, 700},
    { 731, 762, 790, 821, 851, 882, 912, 943, 974,1004,1035,1065},
    {1096,1127,1155,1186,1216,1247,1277,1308,1339,1369,1400,1430},
};

unsigned int getTime(int timezone)
{
	unsigned char sdatetime7[7];
	//char messageBuffer[20];
	int epoch_timezone = timezone*60*60;
					
    //                                                                                                               Y              M                D             H         M             SS              
	if (sys_get_datetime(sdatetime7) >0) //rtc=isl1208.h YYMMDDHH24MISS "20%02d-%02d-%02d %02d:%02d:%02d %d %d",sdatetime7[0],sdatetime7[1],sdatetime7[2],sdatetime7[3],sdatetime7[4],sdatetime7[5],sdatetime7[6]
	{  unsigned int second = sdatetime7[5];  // 0-59
       unsigned int minute = sdatetime7[4];  // 0-59
       unsigned int hour   = sdatetime7[3];    // 0-23
       unsigned int day    = sdatetime7[2]-1;   // 0-30
       unsigned int month  = sdatetime7[1]-1; // 0-11
       unsigned int year   = sdatetime7[0];    // 0-99
	   
	   //rtc_to_short_string(sdatetime7,messageBuffer); // 16-10-10 10:59:59
	   //ShowProgressMessage(messageBuffer, 0, 0);
	   
	   return (((year/4*(365*4+1)+days[year%4][month]+day)*24+hour)*60+minute)*60+second+Y2K-epoch_timezone;
				
	} else {
	  debug_printstring(4, "getTime Error: ", "NO DATE !!!");
	  return 0;
	}
	
}

unsigned int getPrevHB(unsigned int currentDT,unsigned int startHB,unsigned int periodHB){
	return (currentDT -((currentDT-startHB) % periodHB));
}

unsigned int getNextHB(unsigned int currentDT,unsigned int startHB,unsigned int periodHB){
	return (currentDT -((currentDT-startHB) % periodHB) + periodHB);
}

void epoch_to_date_time(date_time_t* date_time,unsigned int epoch,int timezone)
{
	epoch=epoch-Y2K+(timezone*60*60);
    date_time->second = epoch%60; epoch /= 60;
    date_time->minute = epoch%60; epoch /= 60;
    date_time->hour   = epoch%24; epoch /= 24;

    unsigned int years = epoch/(365*4+1)*4; epoch %= 365*4+1;

    unsigned int year;
    for (year=3; year>0; year--)
    {
        if (epoch >= days[year][0])
            break;
    }

    unsigned int month;
    for (month=11; month>0; month--)
    {
        if (epoch >= days[year][month])
            break;
    }

    date_time->year  = years+year;
    date_time->month = month+1;
    date_time->day   = epoch-days[year][month]+1;
}

unsigned int getNextCFggG(void){
	unsigned int currentTime = getTime(8);
	if (isFirst==1){
		
		srand(time(NULL)); // randomize seed	
		unsigned int nextTime = 0;
		do {
			//nextTime = getNextHB(currentTime,0,((rand()%10)*HOUR)); //if nextTime = 0	
			nextTime = (rand()%10)*HOUR;
		} while (nextTime == 0);
		
		FirstTime = nextTime;
		isFirst = 0;
		return nextTime;
		
	}else{
		//return getNextHB(currentTime,FirstTime,(24*HOUR));
		//return (FirstTime+(24*HOUR));
		return FirstTime;
	}
}

unsigned int random_shift=0; 
unsigned int getNextCFG(unsigned int currentDT)
{
	if (random_shift==0)
	{
	  // TODO find a value between 1 and 7200
	  random_shift=1244;	
	};
	
	//return (currentDT -((currentDT-startHB) % periodHB) + periodHB);
    return (currentDT -((currentDT-(0+random_shift)) % 86400) + 86400);
}		
		
	

