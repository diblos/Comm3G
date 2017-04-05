/*
 * main.c
 *
 * Created: 2012-04-09 4:35:26 PM
 *  Author: Justin
 * 1.0.0.0		-			CW		First Release
 */ 
#include <avr32/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "debug.h"
#include "font.h"
#include "color.h"
#include "h2framework.h"
#include "h2core.h"
#include "wifi.h"
#include "util.h"
#include "scom.h"
#include "tcp.h"
#include "fposix.h"
#include "scpi.h"
#include "md5.h"

#define bool int
#define true 1
#define false 0

#define ACK "ACK"
#define NACK "NACK"

char data[] = "OTA:2{s:3:DEV;s:16:0001010053415931;}";
//==================================================
//char fname[] = "ContactlessCard.bin";
//char crc16[] = "7cfb0e5e7173d0e3f94c0bbde0c09682";
//int fsize = 218144;
//--------------------------------------------------
char fname[40];
char crc16[2];
int fsize = 0;
//==================================================
unsigned char tmpMsg[100];

extern int reset_arm(void);//reset arm controller

void ShowProgressMessage(char* message1, char* message2, unsigned char beeptone)
{
	#define xStatus 0
	#define yStatus	224
	#define widthStatus 320
	#define heightStatus 16
	
	if(beeptone != 0)
		beepstd();
	lcd_draw_fillrect(xStatus, yStatus, widthStatus, heightStatus, BLACK);
	if(message1 != 0)
		lcd_draw_string(message1, font_Fixesys16, xStatus, yStatus, WHITE, TRANSPARENT);
	if(message2 != 0)
		lcd_draw_string(message2, font_Fixesys16, xStatus+160, yStatus, WHITE, TRANSPARENT);
}

int scom_Disconnect(){
	int scomDisconnectSt = 0;
	
	ShowProgressMessage("Turn Off 3G", 0, 0);
	
	scomDisconnectSt = scom_set_ppp(0,0,0,0);
	
	ShowProgressMessage("Turning Off 3G", 0, 0);
	sleep(2000);
	if(scomDisconnectSt == 0){
		ShowProgressMessage("Turn Off 3G-ed", 0, 0);	
		sleep(2000);
		return 0;
	}else{
		ShowProgressMessage("Turn Off 3G Failed", 0, 1);
		return -1;
	}
}

int scom_Status(struct_pppStatus* outPppStatus){
	int scomFlagStatus = 0;
	char bufferError[10]={NULL};
	char bufferSignal[10]={NULL};
	
	ShowProgressMessage("3G Status", 0, 0);
	
	scomFlagStatus = scom_get_status_flags(outPppStatus);

	ShowProgressMessage("3G Checking Status", 0, 0);
	if(scomFlagStatus == 0){
		//display all Status Out
		lcd_draw_fillrect(120, 30, 80, 130, ORANGE);
		lcd_draw_string(outPppStatus->active, font_Fixesys16, 120, 30, BLACK, TRANSPARENT);
		sprintf(bufferError,"%d",outPppStatus->Error);
		lcd_draw_string(bufferError, font_Fixesys16, 120, 45, BLACK, TRANSPARENT);
		lcd_draw_string(outPppStatus->dev, font_Fixesys16, 120, 60, BLACK, TRANSPARENT);
		lcd_draw_string(outPppStatus->module, font_Fixesys16, 120, 75, BLACK, TRANSPARENT);
		sprintf(bufferSignal,"%d",outPppStatus->Signal);
		lcd_draw_string(bufferSignal, font_Fixesys16, 120, 90, BLACK, TRANSPARENT);
		lcd_draw_string(outPppStatus->APN, font_Fixesys16, 120, 105, BLACK,TRANSPARENT);
		lcd_draw_string(outPppStatus->userId, font_Fixesys16, 120, 120, BLACK, TRANSPARENT);
		lcd_draw_string(outPppStatus->Password, font_Fixesys16, 120, 135, BLACK, TRANSPARENT);
		return 0;
	}else{
		return 1;
	}					
}

int scom_on_ppp(){
	int scomConnectSt = 0;
	
	ShowProgressMessage("3G Turn On", 0, 0);
	
	scomConnectSt = scom_set_ppp(1,0,0,0);
	
	ShowProgressMessage("3G Turning On", 0, 0);  
	
	return scomConnectSt;
	/*
	if(scomConnectSt == 0){
		sleep(60000);      //Wait 1.10min to turn on 3G
		sleep(10000);
		ShowProgressMessage("3G On!!!", 0, 0);
		sleep(500);
		return 0;
	}else{
		ShowProgressMessage("3G Turn On Failed", 0, 1);
		return -1;
	}*/
}

int scom_Tcp_Connect(int* outTCPID,char* ipAddress,char* port){
	int tcpCID = 0;
	
	ShowProgressMessage("TCP Connect", 0, 0);
	tcpCID = tcp_connect(ipAddress, port);
	ShowProgressMessage("TCP Connecting", 0, 0);
	if(tcpCID>0){
		*outTCPID = tcpCID;
		return 0;
	}else{
		ShowProgressMessage("TCP Connect Fail", 0, 1);
	}
	ShowProgressMessage("TCP Connected", 0, 0);	
}

int scom_Tcp_Write(int tcpCID,char* data,int datalen){
	int scomtcpWriteSt = 0;
	
	ShowProgressMessage("TCP Write", 0, 0);
	scomtcpWriteSt = tcp_write(tcpCID,data,datalen);
	ShowProgressMessage("TCP Writing", 0 ,0);
	if(scomtcpWriteSt == 0){
		ShowProgressMessage("TCP Successful Write", 0, 0);
		return 0;
	}else{
		ShowProgressMessage("TCP Write Fail", 0, 1);
	}
}

int scom_Tcp_Read(int* intcpCID){
	char* respData = 0;
	unsigned char bufData[100] = {0};
	int tcpReadLength = 0;
	int i = 0;
	
	unsigned char msgBuffer[100];
	
	ShowProgressMessage("Start scom_Tcp_Read", 0, 0);sleep(1000);
	
	tcpReadLength = tcp_read(intcpCID,&respData, 10000);
	
	if (tcpReadLength==1){
			switch(respData[0]) {
				case 0X30  :// Acknowledge OK
					return ACK;
					ShowProgressMessage("OK", 0 ,0);sleep(1000);	
					break; /* optional */
	
				case 0X31  :// Acknowledge NOT OK
					ShowProgressMessage("NOT OK", 0 ,0);sleep(1000);
					return NACK;
					break; /* optional */
  
				/* you can have any number of case statements */
				default : /* Optional */
					ShowProgressMessage("Invalid response", 0 ,0);sleep(1000);
					return NACK;
			}
	}else if(tcpReadLength>=5){
			char tag[100][10];
			int tagno = 0;
			//
			//// explode the strint
			//// 2 tages, than by 3, and 3
			for(i = 0; i<tcpReadLength; i++){
	            // ; : { }
				if(respData[i]==0X7C){
					strcpy(tag[tagno++],bufData);
					memset(bufData,0,sizeof(bufData));
				}else{
					// normal, copy by char
					sprintf(&bufData[strlen(bufData)], "%c", respData[i]);
				}
				
			}// tagno=1
			sprintf(crc16,"%s",tag[0]);// CRC CHAR
			sprintf(fname,"%s",tag[1]);// FILENAME
			fsize = atoi(bufData);// LAST DATA
		//return ACK;
	}
	
	tcp_flush(*intcpCID, tcpReadLength);

}

int SendAcknowledgement(int tcpCID,char * AckValue){
	char toSendCommands[50];
	int scomTcpWriteStatus = 0;
	sprintf(toSendCommands,"%s",AckValue);
	scomTcpWriteStatus = scom_Tcp_Write(tcpCID,toSendCommands,strlen(toSendCommands));
	return scomTcpWriteStatus;
}

int TcpReadBytesToFile(int intcpCID,int filedesc){
	char* bufData = 0;
	int tcpReadLength = 0;
	int i = 0;
	int TIMEOUT = 60000;// TIMEOUT IN MILISECONDS
	unsigned char msgBuffer[100];
	int TOTALFILESIZE = fsize;
	bool FINISH = false;
	bool ABORT_ERR = false;
	int TOTALFILESIZE_RCV = 0;
	ShowProgressMessage("Start scom_Tcp_Read", 0, 0);
	
	if(filedesc>=0){
		do {
			 
			 ShowProgressMessage("read the socket", 0, 0);
			  tcpReadLength = tcp_read(&intcpCID,&bufData,TIMEOUT);
			  sprintf(msgBuffer,"just read %d bytes",tcpReadLength);	ShowProgressMessage(msgBuffer, 0, 0);sleep(5000);
			  if (tcpReadLength > 0)
			  {   
				  TOTALFILESIZE_RCV += tcpReadLength;
				  ShowProgressMessage("write buffer into file", 0, 0);sleep(2000);
				  int result = pfwrite(filedesc,bufData,tcpReadLength);// PUT IN FILE
				  if (result==TOTALFILESIZE_RCV){
						if ((TOTALFILESIZE-result)==0) {
							ShowProgressMessage("No more data!", 0, 0);sleep(2000);
							FINISH = true;
						}else if ((TOTALFILESIZE-result)>0){
							sprintf(msgBuffer,"Data left = %d",TOTALFILESIZE-result);	ShowProgressMessage(msgBuffer, 0, 0);sleep(2000);
							}else
							{
								//to do:how  can this be and what to do
								ABORT_ERR =true;
								
							}
						
				  }else if(result==0){
						/*TODO retry to write ?*/
						sprintf(msgBuffer,"Error file! errcode =%d",result); ShowProgressMessage(msgBuffer, 0, 0);sleep(2000);
						ABORT_ERR =true;
				  }else{
					  // positive corrupt file or a error
					  sound_negative();
					  sprintf(msgBuffer,"Error writing file! errcode =%d",result); ShowProgressMessage(msgBuffer, 0, 0);sleep(300000);
					  ABORT_ERR =true;
				  }
				  //sleep(1000);//MANDATORY
				  
			  }else if(tcpReadLength==0){
			  
			      //normal disconnect
				  ShowProgressMessage("Normal disconnect!", 0, 0);sleep(2000);
				  FINISH=true;
				  
			  }else{
				  sound_negative();
				  sprintf(msgBuffer,"Error reading socket! errcode =%d",tcpReadLength);ShowProgressMessage(msgBuffer, 0, 0);sleep(300000);
				  ABORT_ERR =true;
			  }
			  
			  ShowProgressMessage("Flush buffer", 0, 0);sleep(2000);
			  tcp_flush(&intcpCID, tcpReadLength);// TCP FLUSH
			  
			  ShowProgressMessage("Acknowledging", 0, 0);sleep(2000);
			  SendAcknowledgement(intcpCID,ACK);
			  
			  ShowProgressMessage("End of Loop", 0, 0);sleep(2000);
			  
			  /*if (FINISH==true)break;*/
			  
		} while ((tcpReadLength>=0) && (!FINISH)&&(!ABORT_ERR)); // (tcpReadLength!=0);
			
	} // if(filedesc>=0)
	else
    {
		//todo
		NULL;
		
	}		
	
	
	//todo : return error code depending on error, abort or success
}

int scom_Tcp_Disconnect(int inTcpId){
	int scomTcpDisconnectSt = 0;
	
	ShowProgressMessage("TCP Disconnect", 0, 0);
	scomTcpDisconnectSt = tcp_disconnect(inTcpId);
	ShowProgressMessage("TCP Disconnecting", 0, 0);
	
	if(scomTcpDisconnectSt == 0){
		ShowProgressMessage("TCP Successful Disconnected", 0, 0);
		sleep(1000);
	}else{
		ShowProgressMessage("TCP Failed Disconnect", 0, 1);
		sleep(5000);
	}
}
//http://www.lammertbies.nl/comm/info/crc-calculation.html
//http://srecord.sourceforge.net/crc16-ccitt.html
unsigned short ccrc16(const unsigned char* data_p, unsigned char length){// CCITT
    unsigned char x;
    unsigned short crc = 0xFFFF;

    while (length--){
        x = crc >> 8 ^ *data_p++;
        x ^= x>>4;
        crc = (crc << 8) ^ ((unsigned short)(x << 12)) ^ ((unsigned short)(x <<5)) ^ ((unsigned short)x);
    }
    return crc;
}

static void MDPrint (MD5_CTX *mdContext)
{
  unsigned char bufData[100] = {0};
  int i;
  for (i = 0; i < 16; i++){
   // printf ("%02x", mdContext->digest[i]);
	//sprintf(tmpMsg,"%02x",mdContext->digest[i]);ShowProgressMessage(tmpMsg, 0, 0);sleep(1000);
	sprintf(&bufData[strlen(bufData)], "%02x", mdContext->digest[i]);
  }	
  sprintf(tmpMsg,"%s",bufData);ShowProgressMessage(tmpMsg, 0, 0);sleep(60000);sleep(60000);sleep(60000);sleep(60000);
}

void MDString (char *inString)
{
  MD5_CTX mdContext;
  unsigned int len = strlen (inString);

  MD5Init (&mdContext);
  MD5Update (&mdContext, inString, len);
  MD5Final (&mdContext);
  MDPrint (&mdContext);
  //printf (" \"%s\"\n\n", inString);
  sprintf(tmpMsg,"MD5 = %s",inString);ShowProgressMessage(tmpMsg, 0, 0);sleep(1500);
}

void MDFile (char *filename)
{
  int filedesc = pfopen(filename, O_READ);
  if (filedesc<0) {
	sprintf(tmpMsg,"%s can't be opened.\n",filename);ShowProgressMessage(tmpMsg, 0, 0);sleep(60000);
    return 0;
  }
  
  MD5_CTX mdContext;
  int bytes;
  unsigned char data[1024];// todo replace by malloc
  
  MD5Init (&mdContext);
  int count=0;
  while ((bytes = pfread(filedesc, count*1024, 1024, &data)) > 0){
	count++;
	sprintf(tmpMsg,"%d : filesize = %d",count,bytes);ShowProgressMessage(tmpMsg, 0, 0);sleep(2000);
	ShowProgressMessage("Calling MD5Update", 0, 0);	  
	MD5Update (&mdContext, &data, bytes);  
	ShowProgressMessage("End of Loop", 0, 0);sleep(2000);	  
  }
  MD5Final (&mdContext);
  MDPrint (&mdContext);
  pfclose(filedesc);
  return 0;
}

int CheckFileCRC(int pid){
	unsigned char* ndata;
	//unsigned char msgBuffer[100];
	//int filesize = pfgetsize(pid);
	
	unsigned char* Crc16;
	
	int filesize = pfread(pid, 0, 0, &ndata);
	framing_generate_crc16(ndata, filesize, Crc16);
	
	sprintf(tmpMsg,"filesize = %d",filesize);ShowProgressMessage(tmpMsg, 0, 0);sleep(1500);
	sprintf(tmpMsg,"crc = %02X:%02X:%02X",Crc16[0],Crc16[1],Crc16[2]);ShowProgressMessage(tmpMsg, 0, 0);sleep(1500);
	//return 0;
}

//
// START:
//
void (*entry)(void);
int main(void)
{
	//init length y axis display	
	int y_lcd = 30;
	int y_lcdst = 30;
	
	unsigned char msgBuffer[100];

	int tcpCID;
	int counterDialing = 0;
	int scomDisconnectStatus = 0;
	int scomFlagStatus = 0;
	int scomConnectStatus = 0;
	int scomTcpConnectStatus = 0;
	int scomTcpWriteStatus = 0;
	int scomTcpReadStatus = 0;
	int scomTcpReadBytesStatus = 0; // scomTcpReadBytesStatus for bytes reading
	int scomTcpDisconnectStatus = 0;
	struct_pppStatus outPppStatus;
	//char ipAddress[] = "192.168.40.110";
	//char port[] = "57000";
	
	char ipAddress[] = "rdp.seamcloud.com";
	char port[] = "33223";

	
	unsigned int iTickWaitPppOn = 0;
	
	//
	// Init all hardware
	//
	h2core_init(0);
	
	h2core_set_debug(H2HW_3G|H2HW_TTL);
	//h2core_set_debug(H2HW_3G|H2HW_USB|H2HW_TTL);
	h2core_set_debug_console_output(H2HW_SERIAL);
	
	//Draw Something
	lcd_draw_fillrect(0, 0, LCD_WIDTH, LCD_HEIGHT, ORANGE);
	lcd_draw_string("3G COMM", font_MsSerif24, 0, 0, BLACK, TRANSPARENT);
	
	lcd_draw_string("Active  :", font_Fixesys16, 0, y_lcd, BLACK, TRANSPARENT);
	y_lcd += 15;
	lcd_draw_string("Error   :", font_Fixesys16, 0, y_lcd, BLACK, TRANSPARENT);
	y_lcd += 15;
	lcd_draw_string("Dev     :", font_Fixesys16, 0, y_lcd, BLACK, TRANSPARENT);
	y_lcd += 15;
	lcd_draw_string("Module  :", font_Fixesys16, 0, y_lcd, BLACK, TRANSPARENT);
	y_lcd += 15;
	lcd_draw_string("Signal  :", font_Fixesys16, 0, y_lcd, BLACK, TRANSPARENT);
	y_lcd += 15;
	lcd_draw_string("APN     :", font_Fixesys16, 0, y_lcd, BLACK, TRANSPARENT);
	y_lcd += 15;
	lcd_draw_string("UserID  :", font_Fixesys16, 0, y_lcd, BLACK, TRANSPARENT);
	y_lcd += 15;
	lcd_draw_string("Password:", font_Fixesys16, 0, y_lcd, BLACK, TRANSPARENT);
	
	h2core_task();//to turn off watchdog
	
	sleep(2000);
	reset_arm();
	//////////////////////////////////////////////////////////////////////////
	//ShowProgressMessage(crc16, 0, 0);sleep(1000);
	//ShowProgressMessage(fname, 0, 0);sleep(1000);
	//sprintf(msgBuffer,"%d",fsize);	ShowProgressMessage(msgBuffer, 0, 0);sleep(1000);
	//
	//int filedesc = pfopen(fname, O_READ);
	//CheckFileCRC(filedesc);
	//pfclose(filedesc);
	
	//MDFile(fname);
	//unsigned short CS = ccrc16("0123456789",strlen("0123456789"));
	//sprintf(msgBuffer,"%hu",CS);	ShowProgressMessage(msgBuffer, 0, 0);sleep(1000);
	//sprintf(msgBuffer,"%c",CS);	ShowProgressMessage(msgBuffer, 0, 0);sleep(1000);

	//echo dechex(CRC16Normal(get_hexbyte("100000000000000012510908001800040214000c000c021c0002000000000000")));	
	//int x = get_hexbyte("100000000000000012510908001800040214000c000c021c0002000000000000"));
	
	//hexarray_to_hexstring(unsigned char* data, int data_len, char* hexstring, char spacing);
	//framing_generate_crc16()
		
	//hexarray_to_hexstring(port, strlen(port), x, ":");
	//unsigned char buff[100];hexstring_to_hexarray(crc16, strlen(crc16), buff);
	//sprintf(msgBuffer,"%02X:%02X:%02X",buff[0],buff[1],buff[2]);ShowProgressMessage(msgBuffer, 0, 0);sleep(1000);
	
	//MDString("MEGA");
	//MDFile("myevolution.png");
	
	//h2core_exit_to_main_sector();
	//////////////////////////////////////////////////////////////////////////
	//
	//TURN ON PPP
	//
	RECONNECT:
	scomConnectStatus = scom_on_ppp();
	if(scomConnectStatus != 0){
		//failed to turn on
		//wait 2 sec and try again
		sleep(2000);
		goto RECONNECT;
	}
	iTickWaitPppOn = gettickcount() + 90000;//wait up to 90sec for module to register to network, subject to signal

	
	//Get SCOM Status
	RESTATUS1:
	scomFlagStatus = scom_Status(&outPppStatus);
	if(scomFlagStatus != 0)
	{
		sleep(2000);
		goto SCOMDISCONNECT;
	}
	
	if(strcmp(outPppStatus.active,"On")!= 0)
	{
		if(gettickcount() > iTickWaitPppOn)
		{
			goto SCOMDISCONNECT;
		}
		else
		{
			sleep(1000);
			goto RESTATUS1;
		}
	}
		
		
	//Connect TCP
	RECONNECTTCP:
	scomTcpConnectStatus = scom_Tcp_Connect(&tcpCID,ipAddress,port);
	if(scomTcpConnectStatus != 0){
		goto RECONNECTTCP;	
	}
		
	//Tcp Write
	scomTcpWriteStatus = scom_Tcp_Write(tcpCID,data,strlen(data));
	if(scomTcpWriteStatus != 0){
		goto DISCONNECTTCP;	
	}else{
		sleep(1000);
	}
		
	//Tcp Read
	scomTcpReadStatus = scom_Tcp_Read(&tcpCID);
	if(scomTcpReadStatus != 0){
		goto DISCONNECTTCP;
	}else{
		sleep(1000);
	}

	/*
	ShowProgressMessage(crc16, 0, 0);sleep(1000);
	ShowProgressMessage(fname, 0, 0);sleep(1000);
	sprintf(msgBuffer,"%d",fsize);	ShowProgressMessage(msgBuffer, 0, 0);sleep(1000);
	*/
	
	//char CurrentStatus[] = ACK;
	
	pfdelete(fname);// DELETE PREVIOUS FAILED TRANSFERED FILE IF ANY
	//SendAcknowledgement(tcpCID,CurrentStatus);// SEND ACK / NACK
	
	sleep(1000);
	SendAcknowledgement(tcpCID,ACK);
	
	//if (1==1){
			
		ShowProgressMessage("Opening the file", 0, 0);sleep(1000);
		int filedesc = pfopen(fname, O_CREATE | O_WRITE);
	
		sprintf(msgBuffer,"filedesc=%d",filedesc);	ShowProgressMessage(msgBuffer, 0, 0);
		
		scomTcpReadBytesStatus = TcpReadBytesToFile(tcpCID,filedesc);
		//if(scomTcpReadBytesStatus != 0){
			//goto DISCONNECTTCP;
		//}else{
			//sleep(1000);
		//}
		
		ShowProgressMessage("closing the file", 0, 0);sleep(1000);
		//CheckFileCRC(filedesc);
		pfclose(filedesc);
		
		// todo md5 crc
		MDFile(fname);
	//}

	//TCP Disconnect
	DISCONNECTTCP:	
	scomTcpDisconnectStatus = scom_Tcp_Disconnect(tcpCID);
	if(scomTcpDisconnectStatus != 0){
		goto DISCONNECTTCP;
	}
		
	//SCOM Disconnect
	SCOMDISCONNECT:
	scomDisconnectStatus = scom_Disconnect();



	
	/*	

	while(1){
		
	}*/	
	
	//beepstd();
	
	//
	//END: Exit App, return back to main application sector
	//
	h2core_exit_to_main_sector();
}