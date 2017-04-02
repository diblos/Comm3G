/*
 * scpi.h
 *
 * Created: 2012-03-28 3:18:13 PM
 *  Author: Justin
 */ 


#ifndef SCPI_H_
#define SCPI_H_

//
// Class & Instruction
//

#define SCPI_CLA_SAM					0x02
#define SCPI_CLA_PCD					0x03
#define SCPI_CLA_SYS					0x06
#define SCPI_CLA_FS					0x07
				
#define SCPI_SAM_CONNECT				0x01
#define SCPI_SAM_GET_STATUS			0x02
#define SCPI_SAM_TRANSMIT				0x03
#define SCPI_SAM_DISCONNECT			0x04

#define SCPI_PCD_RF_OFF				0x10//RF OFF
#define SCPI_PCD_RF_ON					0x11//RF ON
#define SCPI_PCD_CHANGE_BAUDRATE_A		0x1A
#define SCPI_PCD_CHANGE_BAUDRATE_B		0x1B
#define SCPI_PCD_SET_TIMEOUT			0x1C
//0x20 to 0x2F - STANDARD ISO14443 PART 3 TYPE A FUNCTION
#define SCPI_PCD_REQA					0x20
#define SCPI_PCD_WUPA					0x21
#define SCPI_PCD_ANTICOL				0x22
#define SCPI_PCD_SELECT				0x23
#define SCPI_PCD_HLTA					0x24
#define SCPI_PCD_REQB					0x2B
#define SCPI_PCD_WUPB					0x2C
//0x30 to 0x3F - MIFARE FUNCITON
#define SCPI_PCD_MI_CHANGECODEKEY 		0x30
#define SCPI_PCD_MI_AUTHKEY			0x31
#define SCPI_PCD_MI_AUTHSTATE			0x32
#define SCPI_PCD_MI_READ				0x33
#define SCPI_PCD_MI_WRITE				0x34
#define SCPI_PCD_MI_VALUE				0x35
#define SCPI_PCD_MI_RESTORE			0x36
#define SCPI_PCD_MI_TRANSFER			0x37
#define SCPI_PCD_MI_AUTH				0x3A
//0x40 to 0x4F - ISO14443 PART4 A FUNCTION
#define SCPI_PCD_TRANSA				0x40
#define SCPI_PCD_TRANSA_CRC			0x41
//0x50 to 0x5F - ISO14443 PART4 B FUNCTION
#define SCPI_PCD_TRANSB				0x50
#define SCPI_PCD_TRANSB_CRC			0x51
//60 to 6F - EMV
#define SCPI_PCD_EMV_POLLING			0x61
#define SCPI_PCD_EMV_LOOPBACK			0x62
#define SCPI_PCD_EMV_SEND_APDU			0x63

//
// System
//
#define SCPI_SYS_XMODEM				0xD1
#define SCPI_SYS_GET_SN				0xE0 
#define SCPI_SYS_GET_DBG_PORT			0xE2 
#define SCPI_SYS_SET_DBG_PORT			0xE3 
#define SCPI_SYS_GET_DBG_LVL			0xE4 
#define SCPI_SYS_SET_DBG_LVL			0xE5 

#define SCPI_SYS_ECHO					0xC0
#define SCPI_SYS_SET_BAUDRATE			0xC1
#define SCPI_SYS_GET_DATETIME			0xC6
#define SCPI_SYS_SET_DATETIME			0xC7

#define SCPI_SYS_GET_FIRMWARE_INFO		0xE7
#define SCPI_SYS_SYSTEM_GET			0xE8

#define SCPI_SYS_BRATE_9600			0x01
#define SCPI_SYS_BRATE_19200			0x02
#define SCPI_SYS_BRATE_38400			0x03
#define SCPI_SYS_BRATE_57600			0x04
#define SCPI_SYS_BRATE_115200			0x05

//
// File System (FS)
//
#define SCPI_FS_LIST_FILE				0x01
#define SCPI_FS_OPEN					0x02
#define SCPI_FS_READ					0x03
#define SCPI_FS_WRITE					0x04
#define SCPI_FS_CLOSE					0x0A
#define SCPI_FS_DELETE					0x0D
#define SCPI_FS_FORMAT					0x0F
#define SCPI_FS_LOW_LEVEL_FORMAT		0x1F



/*
#define SCPI_PCDA_HUMAN				0x01	//Human interface
#define SCPI_PCDA_CONTACT			0x02	//Contact card / SAM
#define SCPI_PCDA_CONTACTLESS		0x03	//Contactless funtion, Mifare, ISO14443A&B
#define SCPI_PCDA_CRYPTO			0x04	//CryptoServiceEngine
#define SCPI_PCDA_RS232				0x05	//RS232 Echo Test/READ/WRITE
#define SCPI_PCDA_SYSTEM			0x06	//System
#define SCPI_PCDA_ADVANCE			0x07    //Advance Command
*/
//
// Error Code
//
#define SCPI_ER_OK						0x00
#define SCPI_ER_FAILED					0xFF
#define SCPI_ER_INVALID_DATA			0xFD
#define SCPI_ER_INVALID_LEN			0xFC
#define SCPI_ER_INVALID_CMD			0xFB
#define SCPI_ER_INVALID_CLASS			0xFA



//
// Extern Functions
//
extern void scpi_task(void);
extern unsigned char scpi_process_command(unsigned char cla, unsigned char cmd, const unsigned char* datain, int datainlen, unsigned char* resp, int* resplen, unsigned char* pStatus);
extern unsigned char scpi_sam_process_command(unsigned char Command, const unsigned char* DataIn, const int DataInLength, unsigned char* Resp, int* RespLength, unsigned char* Status);
extern unsigned char scpi_pcd_process_command(unsigned char Command, const unsigned char* DataIn, const int DataInLength, unsigned char* Resp, int* RespLength, unsigned char* Status);
extern unsigned char scpi_sys_process_command(unsigned char Command, const unsigned char* DataIn, const int DataInLength, unsigned char* Resp, int* RespLength, unsigned char* Status);
extern unsigned char scpi_fs_process_command(unsigned char Command, const unsigned char* DataIn, const int DataInLength, unsigned char* Resp, int* RespLength, unsigned char* Status);

#endif /* SCPI_H_ */