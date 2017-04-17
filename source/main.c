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
#include "gui.h"

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
char md5[32];
int fsize = 0;
int fchunks = 0;
//==================================================
unsigned char tmpMsg[100]={NULL};
unsigned char resultMD5[32] = {NULL};

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

int psCheckSum(char *string)
{
  int XOR = 0;
  //sprintf(tmpMsg,"LEN: %d",strlen(string));ShowProgressMessage(tmpMsg, 0, 0);sleep(1000);
  for (int i = 0; i < strlen(string); i++)
  {
    XOR = XOR ^ string[i];
  }
  return XOR;
}

float ShowPercentage(int current, int total) {
	//float percentage;
	//percentage = (float) current / total * 100.0;
	return (float) current / total * 100.0;
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
		lcd_draw_fillrect(90, 30, 80, 130, ORANGE);
		lcd_draw_string(outPppStatus->active, font_Fixesys16, 90, 30, BLACK, TRANSPARENT);
		sprintf(bufferError,"%d",outPppStatus->Error);
		lcd_draw_string(bufferError, font_Fixesys16, 90, 45, BLACK, TRANSPARENT);
		lcd_draw_string(outPppStatus->dev, font_Fixesys16, 90, 60, BLACK, TRANSPARENT);
		lcd_draw_string(outPppStatus->module, font_Fixesys16, 90, 75, BLACK, TRANSPARENT);
		sprintf(bufferSignal,"%d",outPppStatus->Signal);
		lcd_draw_string(bufferSignal, font_Fixesys16, 90, 90, BLACK, TRANSPARENT);
		lcd_draw_string(outPppStatus->APN, font_Fixesys16, 90, 105, BLACK,TRANSPARENT);
// 		lcd_draw_string(outPppStatus->userId, font_Fixesys16, 120, 120, BLACK, TRANSPARENT);
// 		lcd_draw_string(outPppStatus->Password, font_Fixesys16, 120, 135, BLACK, TRANSPARENT);
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
			char tag[5][100];
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
			sprintf(md5,"%s",tag[0]);// CRC CHAR
			sprintf(fname,"%s",tag[1]);// FILENAME
			fsize = atoi(tag[2]);// LAST DATA
			fchunks = atoi(bufData);// LAST DATA
		//return ACK;
	}
	
	tcp_flush(*intcpCID, tcpReadLength);

}

int SendTCP(int tcpCID,char * AckValue){
	char toSendCommands[50];
	int scomTcpWriteStatus = 0;
	sprintf(toSendCommands,"%s",AckValue);
	scomTcpWriteStatus = scom_Tcp_Write(tcpCID,toSendCommands,strlen(toSendCommands));
	return scomTcpWriteStatus;
}

bool TcpReadBytesToFile_OLD(int intcpCID,char * filename){
	char* bufData = 0;
	int tcpReadLength = 0;
	int i = 0;
	int TIMEOUT = 60000;// TIMEOUT IN MILISECONDS
	int TOTALFILESIZE = fsize;
	bool FINISH = false;
	bool ABORT_ERR = false;
	//bool IS_HALF = false;
	int TOTALFILESIZE_RCV = 0;
	
	sprintf(tmpMsg,"Opening %s for writing...",filename);ShowProgressMessage(tmpMsg, 0, 0);sleep(2000);
	int filedesc = pfopen(filename, O_CREATE | O_WRITE);	
	if (filedesc<0) {
	sprintf(tmpMsg,"%s can't be opened.",filename);ShowProgressMessage(tmpMsg, 0, 0);sleep(60000);
    return false;
	}
	
	ShowProgressMessage("Start scom_Tcp_Read", 0, 0);
	
	if(filedesc>=0){
		do {
			 
			 ShowProgressMessage("read the socket", 0, 0);
			  tcpReadLength = tcp_read(&intcpCID,&bufData,TIMEOUT);
			  sprintf(tmpMsg,"just read %d bytes",tcpReadLength);	ShowProgressMessage(tmpMsg, 0, 0);sleep(5000);
			  if (tcpReadLength > 0)
			  {   
				  TOTALFILESIZE_RCV += tcpReadLength;
				  ShowProgressMessage("write buffer into file", 0, 0);sleep(2000);
				  int result = pfwrite(filedesc,bufData,tcpReadLength);// PUT IN FILE
				  if (result==TOTALFILESIZE_RCV){
						sprintf(tmpMsg,"Progress : %.2f%% ",ShowPercentage(result,TOTALFILESIZE));
						lcd_draw_string(tmpMsg, font_Fixesys16, 3, 200, BLACK,ORANGE);
						if ((TOTALFILESIZE-result)==0) {
							ShowProgressMessage("No more data!", 0, 0);sleep(2000);
							FINISH = true;
						}else if ((TOTALFILESIZE-result)>0){
							//if (result<4096) IS_HALF = true;	
							sprintf(tmpMsg,"Data left = %d",TOTALFILESIZE-result);	ShowProgressMessage(tmpMsg, 0, 0);sleep(2000);
							}else
							{
								//to do:how  can this be and what to do
								ABORT_ERR =true;
								
							}
						
				  }else if(result==0){
						/*TODO retry to write ?*/
						sprintf(tmpMsg,"Error file! errcode =%d",result); ShowProgressMessage(tmpMsg, 0, 0);sleep(2000);
						ABORT_ERR =true;
				  }else{
					  // positive corrupt file or a error
					  sound_negative();
					  sprintf(tmpMsg,"Error writing file! errcode =%d",result); ShowProgressMessage(tmpMsg, 0, 0);sleep(300000);
					  ABORT_ERR =true;
				  }
				  //sleep(1000);//MANDATORY
				  
			  }else if(tcpReadLength==0){
			  
			      //normal disconnect
				  ShowProgressMessage("Normal disconnect!", 0, 0);sleep(2000);
				  FINISH=true;
				  
			  }else{
				  sound_negative();
				  sprintf(tmpMsg,"Error reading socket! errcode =%d",tcpReadLength);ShowProgressMessage(tmpMsg, 0, 0);sleep(300000);
				  ABORT_ERR =true;
			  }
			  
			  ShowProgressMessage("Flush buffer", 0, 0);sleep(2000);
			  tcp_flush(&intcpCID, tcpReadLength);// TCP FLUSH
			  
			  ShowProgressMessage("Acknowledging", 0, 0);sleep(2000);
			  
			  //if (!IS_HALF) SendAcknowledgement(intcpCID,ACK);
			  SendTCP(intcpCID,ACK);
			  
			  ShowProgressMessage("End of Loop", 0, 0);sleep(2000);
			  
			  /*if (FINISH==true)break;*/
			  
		} while ((tcpReadLength>=0) && (!FINISH)&&(!ABORT_ERR)); // (tcpReadLength!=0);
			
	} // if(filedesc>=0)
	else
    {
		//todo
		return false;
	}		
	
	if(ABORT_ERR) return false;
	
	ShowProgressMessage("closing the file", 0, 0);sleep(1000);
	int close = pfclose(filedesc);
	if(close<0) return false;
	
	//todo : return error code depending on error, abort or success
	return true;
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


 void MDPrint (MD5_CTX *mdContext)
{
  unsigned char bufData[100] = {0};
  int i;
  for (i = 0; i < 16; i++){
	sprintf(&bufData[strlen(bufData)], "%02x", mdContext->digest[i]);
  }
  sprintf(resultMD5,"%s",bufData);ShowProgressMessage(resultMD5, 0, 0);sleep(5000);//sleep(60000);sleep(60000);sleep(60000);
}

bool MDString (char *inString , char *verify)
{
	MD5_CTX mdContext;
	unsigned int len = strlen (inString);
	
	MD5Init (&mdContext);
	MD5Update (&mdContext, inString, len);
	MD5Final (&mdContext);
	MDPrint (&mdContext);
	  
	//sprintf(tmpMsg,"MD5 1: %s",resultMD5);ShowProgressMessage(retChr, 0, 0);sleep(5000);
	sprintf(tmpMsg,"ver: %s",verify);ShowProgressMessage(tmpMsg, 0, 0);sleep(5000);
	//ShowProgressMessage("MD5 Failed!", 0, 0);sleep(2000);
  //
	if(strcmp(resultMD5, verify) != 0){
		//ShowProgressMessage("MD5 Failed!", 0, 0);sleep(2000);
		return false;
	}else{
		//ShowProgressMessage("MD5 Succeed!", 0, 0);sleep(2000);
		return true;
	}
	return false;
}

bool MDData (unsigned char *inData , char *verify)
{
	MD5_CTX mdContext;
	//unsigned int len = strlen (inData);
	unsigned int len = sizeof (inData);
	
	sprintf(tmpMsg,"size: %d",len);ShowProgressMessage(tmpMsg, 0, 0);sleep(2000);
		
	MD5Init (&mdContext);
	MD5Update (&mdContext, &inData, len);
	MD5Final (&mdContext);
	MDPrint (&mdContext);
	  
	sprintf(tmpMsg,"ver: %s",verify);ShowProgressMessage(tmpMsg, 0, 0);sleep(5000);
 
	if(strcmp(resultMD5, verify) != 0){
		//ShowProgressMessage("MD5 Failed!", 0, 0);sleep(2000);
		return false;
	}else{
		//ShowProgressMessage("MD5 Succeed!", 0, 0);sleep(2000);
		return true;
	}
	return false;
}

bool MDFile (char *filename)
{
  sprintf(tmpMsg,"Checking %s MD5...",filename);ShowProgressMessage(tmpMsg, 0, 0);sleep(2000);
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
	/*sprintf(tmpMsg,"%d : filesize = %d",count,bytes);ShowProgressMessage(tmpMsg, 0, 0);sleep(2000);*/
	/*ShowProgressMessage("Calling MD5Update", 0, 0);	  */
	MD5Update (&mdContext, &data, bytes);  
	/*ShowProgressMessage("End of Loop", 0, 0);sleep(2000);*/
  }
  MD5Final (&mdContext);
  MDPrint (&mdContext);
  
  if(strcmp(resultMD5, md5) != 0){
	  ShowProgressMessage("MD5 Failed!", 0, 0);sleep(2000);
	  return false;
  }else{
	  ShowProgressMessage("MD5 Succeed!", 0, 0);sleep(2000);
	  return true;
  }
  
  pfclose(filedesc);
  
  return 0;
}

bool TcpReadBytesToFile(int intcpCID,char * filename){

	int tcpReadLength1 = 0;int tcpReadLength2 = 0;
	int i = 0;int j = 0;
	int TIMEOUT = 60000;// TIMEOUT IN MILISECONDS
	int TOTALFILESIZE = fsize;
	bool FINISH = false;
	bool ABORT_ERR = false;
	
	int TOTALFILESIZE_RCV = 0;
	
	sprintf(tmpMsg,"Opening %s for writing...",filename);ShowProgressMessage(tmpMsg, 0, 0);sleep(2000);
	int filedesc = pfopen(filename, O_CREATE | O_WRITE);	
	if (filedesc<0) {
	sprintf(tmpMsg,"%s can't be opened.",filename);ShowProgressMessage(tmpMsg, 0, 0);sleep(60000);
    return false;
	}
	//================================================================================================================================================
	
		int scomTcpWriteStatus = 0;
	
	    for (i=1; i <= fchunks ; i++){
			char* bufData = 0;
			unsigned char tmpData[100] = {0};
			int chunksize = 0;char chunkcrc[32];int intcs=0;
					
			sprintf(tmpMsg,"PROP|%d",i);ShowProgressMessage(tmpMsg, 0, 0);sleep(1000);
			SendTCP(intcpCID,tmpMsg);// REQUEST CHUNK PROPERTY
						
			tcpReadLength1 = tcp_read(&intcpCID,&bufData,TIMEOUT);	

			if (tcpReadLength1 > 0){// HARDCODED STRING SIZE
				char tag[3][100];
				int tagno = 0;
				
				for(j = 0; j<tcpReadLength1; j++){//EXPLODE
					if(bufData[j]==0X7C){// '|'
						strcpy(tag[tagno++],tmpData);
						memset(tmpData,0,sizeof(tmpData));
					}else{
						sprintf(&tmpData[strlen(tmpData)], "%c", bufData[j]);
					}				
				}// tagno=1
				chunksize = atoi(tag[0]);// CHUNK SIZE
				intcs = atoi(tag[1]);// CRC INT
				//sprintf(chunkcrc,"%s",tmpData);// CRC STR
				
				//sprintf(tmpMsg,"len:%d | cs:%s",chunksize,chunkcrc);ShowProgressMessage(tmpMsg, 0, 0);sleep(2000);
				sprintf(tmpMsg,"len:%d | cs:%d",chunksize,intcs);ShowProgressMessage(tmpMsg, 0, 0);sleep(2000);
				
				tcp_flush(&intcpCID, tcpReadLength1);// TCP FLUSH
				
				bool RETRY = true;
				while(RETRY){
					
					sprintf(tmpMsg,"GET|%d",i);ShowProgressMessage(tmpMsg, 0, 0);sleep(1000);
					SendTCP(intcpCID,tmpMsg);// REQUEST CHUNK DATA

					tcpReadLength2 = tcp_read(&intcpCID,&bufData,TIMEOUT);
					//SANITY CHECKING STARTS
					
					//sprintf(tmpMsg,"size : %d|%d",tcpReadLength2,chunksize);ShowProgressMessage(tmpMsg, 0, 0);sleep(2000);	
					if (tcpReadLength2 == chunksize){
						////int CS = psCheckSum(get_hexchar(bufData));
						//bool CS = MDData(&bufData,chunkcrc);
						//
						//if(CS==true){
							int result = pfwrite(filedesc,bufData,tcpReadLength2);// PUT IN FILE
														//
							sprintf(tmpMsg,"Progress : %.2f%% ",ShowPercentage(result,TOTALFILESIZE));
							lcd_draw_string(tmpMsg, font_Fixesys16, 3, 200, BLACK,ORANGE);
							
							if ((TOTALFILESIZE-result)==0) {
								ShowProgressMessage("No more data!", 0, 0);sleep(1000);
								SendTCP(intcpCID,ACK);// SEND ACK
								RETRY=false;
							}else{
								RETRY=false;
							}							
						//}else{
							//ShowProgressMessage("checksum mismatched! Retrying...", 0, 0);sleep(1000);
							//RETRY = true;
						//}
						
					}else{
						sprintf(tmpMsg,"chunk size mismatched [%d of %d]! Retrying...",tcpReadLength2,chunksize);
						ShowProgressMessage(tmpMsg, 0, 0);sleep(1000);
						RETRY = true;
					}
					//SANITY CHECKING ENDS
					tcp_flush(&intcpCID, tcpReadLength2);// TCP FLUSH
				}
			}
			
		}
		ShowProgressMessage("closing the file", 0, 0);sleep(1000);
		int close = pfclose(filedesc);
	
		return true;
	//================================================================================================================================================
	/*
	ShowProgressMessage("Start scom_Tcp_Read", 0, 0);
	
	if(filedesc>=0){
		do {
			 
			 ShowProgressMessage("read the socket", 0, 0);
			  tcpReadLength = tcp_read(&intcpCID,&bufData,TIMEOUT);
			  sprintf(tmpMsg,"just read %d bytes",tcpReadLength);	ShowProgressMessage(tmpMsg, 0, 0);sleep(5000);
			  if (tcpReadLength > 0)
			  {   
				  TOTALFILESIZE_RCV += tcpReadLength;
				  ShowProgressMessage("write buffer into file", 0, 0);sleep(2000);
				  int result = pfwrite(filedesc,bufData,tcpReadLength);// PUT IN FILE
				  if (result==TOTALFILESIZE_RCV){
						sprintf(tmpMsg,"Progress : %.2f%% ",ShowPercentage(result,TOTALFILESIZE));
						lcd_draw_string(tmpMsg, font_Fixesys16, 3, 200, BLACK,ORANGE);
						if ((TOTALFILESIZE-result)==0) {
							ShowProgressMessage("No more data!", 0, 0);sleep(2000);
							FINISH = true;
						}else if ((TOTALFILESIZE-result)>0){
							//if (result<4096) IS_HALF = true;	
							sprintf(tmpMsg,"Data left = %d",TOTALFILESIZE-result);	ShowProgressMessage(tmpMsg, 0, 0);sleep(2000);
							}else
							{
								//to do:how  can this be and what to do
								ABORT_ERR =true;
								
							}
						
				  }else if(result==0){
						//TODO retry to write ?
						sprintf(tmpMsg,"Error file! errcode =%d",result); ShowProgressMessage(tmpMsg, 0, 0);sleep(2000);
						ABORT_ERR =true;
				  }else{
					  // positive corrupt file or a error
					  sound_negative();
					  sprintf(tmpMsg,"Error writing file! errcode =%d",result); ShowProgressMessage(tmpMsg, 0, 0);sleep(300000);
					  ABORT_ERR =true;
				  }
				  //sleep(1000);//MANDATORY
				  
			  }else if(tcpReadLength==0){
			  
			      //normal disconnect
				  ShowProgressMessage("Normal disconnect!", 0, 0);sleep(2000);
				  FINISH=true;
				  
			  }else{
				  sound_negative();
				  sprintf(tmpMsg,"Error reading socket! errcode =%d",tcpReadLength);ShowProgressMessage(tmpMsg, 0, 0);sleep(300000);
				  ABORT_ERR =true;
			  }
			  
			  ShowProgressMessage("Flush buffer", 0, 0);sleep(2000);
			  tcp_flush(&intcpCID, tcpReadLength);// TCP FLUSH
			  
			  ShowProgressMessage("Acknowledging", 0, 0);sleep(2000);
			  
			  //if (!IS_HALF) SendAcknowledgement(intcpCID,ACK);
			  SendAcknowledgement(intcpCID,ACK);
			  
			  ShowProgressMessage("End of Loop", 0, 0);sleep(2000);
			  
			  //if (FINISH==true)break;
			  
		} while ((tcpReadLength>=0) && (!FINISH)&&(!ABORT_ERR)); // (tcpReadLength!=0);
			
	} // if(filedesc>=0)
	else
    {
		//todo
		return false;
	}		
	
	if(ABORT_ERR) return false;
	
	ShowProgressMessage("closing the file", 0, 0);sleep(1000);
	int close = pfclose(filedesc);
	if(close<0) return false;
	
	//todo : return error code depending on error, abort or success
	return true;
	*/
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
	
	char ipAddress[] = "192.168.40.110";char port[] = "57000";
	
	//char ipAddress[] = "rdp.seamcloud.com";	char port[] = "33223";

	
	unsigned int iTickWaitPppOn = 0;
	
	//
	// Init all hardware
	//
	h2core_init(0);
	
	//h2core_set_debug(H2HW_3G|H2HW_TTL);
	//h2core_set_debug(H2HW_3G|H2HW_USB|H2HW_TTL);
	//h2core_set_debug_console_output(H2HW_SERIAL);
	
	//Draw Something
	lcd_draw_fillrect(0, 0, LCD_WIDTH, LCD_HEIGHT, ORANGE);
	lcd_draw_string("OTA Update", font_MsSerif18, 3, 0, BLACK, TRANSPARENT);
	
	lcd_draw_string("Active  :", font_Fixesys16, 3, y_lcd, BLACK, TRANSPARENT);
	y_lcd += 15;
	lcd_draw_string("Error   :", font_Fixesys16, 3, y_lcd, BLACK, TRANSPARENT);
	y_lcd += 15;
	lcd_draw_string("Dev     :", font_Fixesys16, 3, y_lcd, BLACK, TRANSPARENT);
	y_lcd += 15;
	lcd_draw_string("Module  :", font_Fixesys16, 3, y_lcd, BLACK, TRANSPARENT);
	y_lcd += 15;
	lcd_draw_string("Signal  :", font_Fixesys16, 3, y_lcd, BLACK, TRANSPARENT);
	y_lcd += 15;
	lcd_draw_string("APN     :", font_Fixesys16, 3, y_lcd, BLACK, TRANSPARENT);
	
	h2core_task();//to turn off watchdog
	
	//sleep(2000);
	reset_arm();
	//////////////////////////////////////////////////////////////////////////
	//char a[] = "2929B100070A9F95380C820D";
	//char v[] = "3c14a7285e2d0c7e326f6eb72e7c84eb";
	//
	//bool result = MDString(a,v);
	//if(result){
	//ShowProgressMessage("SUCCESS", 0, 0);sleep(1000);
	//}else{
	//ShowProgressMessage("FAIL", 0, 0);sleep(1000);}
	
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
	
		
// 	progressbar pbar = {"A",1,0,0,BLUE,WHITE};
// 	int x = 0;
// 	for(x=0;x<=10;x++){
// 		gui_draw_progress(&pbar, x);sleep(1000);
// 	}
// 	
//unsigned char bufData[100] = {0};
//int a[] = {0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0x51, 0x09, 0x08, 0x00, 0x18, 0x00, 0x04, 0x02, 0x14, 0x00, 0x0c, 0x00, 0x0c, 0x02, 0x1c, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
//unsigned char a[]={0};
//char b[] = "100000000000000012510908001800040214000c000c021c0002000000000000";
//char b[] = "10 00 00 00 00 00 00 00 12 51 09 08 00 18 00 04 02 14 00 0c 00 0c 02 1c 00 02 00 00 00 00 00 00";

	
	//lcd_draw_string(b, font_default8, 3, 200, BLACK, TRANSPARENT);
	
	//c = get_hexbyte(b);
	
	//hexstring_to_hexarray(&b, strlen(b), &a);
	//sprintf(tmpMsg,">> %d",strlen(c));ShowProgressMessage(tmpMsg, 0, 0);sleep(20000);
	
	
	//int theCount = strlen(a);int i;
	 //for (i = 0; i < theCount; i++){
	//sprintf(&bufData[strlen(bufData)], "%02x", a[i]);	
  //}
  
  //hexarray_to_hexstring(&a, strlen(a), &bufData, ":");
  
	//framing_generate_crc16(a,strlen(a),&bufData);
	
	//unsigned short w = Getcrc16(&b, strlen(b));
	
	
	//char a[] = "2929B100070A9F95380C820D";
	////char b[] = {0x29,0x29,0xB1,0x00,0x07,0x0A,0x9F,0x95,0x38,0x0C,0x82,0x0D};
	//int output = 0;
	//output = psCheckSum(a);		
	//sprintf(tmpMsg,">> %d",output);ShowProgressMessage(tmpMsg, 0, 0);sleep(5000);
	////output = psCheckSum(b);		
	////sprintf(tmpMsg,">> %d",output);ShowProgressMessage(tmpMsg, 0, 0);sleep(5000);
 	//
	 //
	 //sprintf(tmpMsg,">> %d",get_int("70"));ShowProgressMessage(tmpMsg, 0, 0);sleep(5000);
	 //
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
	
	y_lcd = 120;
	sprintf(msgBuffer,"filename:  %s",fname);
	lcd_draw_string(msgBuffer, font_Fixesys16, 3, y_lcd, BLACK, TRANSPARENT);
	y_lcd += 15;
	sprintf(msgBuffer,"filesize:  %d bytes",fsize);
	lcd_draw_string(msgBuffer, font_Fixesys16, 3, y_lcd, BLACK, TRANSPARENT);
	y_lcd += 15;
	sprintf(msgBuffer,"Chunks  :  %d",fchunks);
	lcd_draw_string(msgBuffer, font_Fixesys16, 3, y_lcd, BLACK, TRANSPARENT);
	y_lcd += 15;
	sprintf(msgBuffer,"MD5: %s",md5);
	lcd_draw_string(msgBuffer, font_Fixesys16, 3, y_lcd, BLACK, TRANSPARENT);	
	//char CurrentStatus[] = ACK;
	
	pfdelete(fname);// DELETE PREVIOUS FAILED TRANSFERED FILE IF ANY
	//SendAcknowledgement(tcpCID,CurrentStatus);// SEND ACK / NACK
		
	//sleep(1000);
	//SendAcknowledgement(tcpCID,ACK);
	
	//if (1==1){
		
		scomTcpReadBytesStatus = TcpReadBytesToFile(tcpCID,fname);
		if(scomTcpReadBytesStatus == false){
			// FILE TRANSMISSION ERROR
			ShowProgressMessage("Update failed!", 0, 0);sleep(1000);
		}else{
			// TODO MD5 CRC
			MDFile(fname);
		}
		

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