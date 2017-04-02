/*
 * iso7816.h
 *
 * Created: 2012-3-1 16:43:24
 *  Author: Guo Wenxue<guowenxue@gmail.com>
 */ 


#ifndef SAM_H_
#define SAM_H_

#define SAM1                                1
#define SAM2                                2
#define SAM3                                3
#define SAM4                                4

#define T0	                                0
#define T1	                                1

#define PPS_9600_FD							372
#define PPS_19200_FD						186
#define PPS_38400_FD						93
#define PPS_76800_FD						46
#define PPS_115200_FD						31

#define SAM_ER_OK							0x00
#define SAM_ER_INVALID_HANDLE				0xE1
#define SAM_ER_TIME_OUT						0xFF
#define SAM_ER_UNEXPECTED_RESP				0xFE
#define SAM_ER_POWERUP_ATR_INVALID			0xFD
#define SAM_ER_DO_WARM_RESET				0xFC
#define SAM_ER_POWERUP_ATR_CRC_FAILURE		0xFB

#define ISO7816_T1_R_OK				0x00	// b0000
#define ISO7816_T1_R_ERROR			0x01	// b0001
#define ISO7816_T1_R_OTHER			0x02	// b0010

extern unsigned char sam_connect(unsigned char slotid);
extern unsigned char sam_get_status(unsigned char slotid, unsigned char** atr, int* atrlen);
extern unsigned char sam_transmit(unsigned char slotid, unsigned char* apdu, int apdulen, unsigned char* resp, int* resplen);
extern unsigned char sam_disconnect(unsigned char slotid);
extern unsigned char sam_pps(unsigned char slotid, unsigned long ppsFdId);

//this function transmit any apdu, length range from 1 to 255 bytes, (for PPS)
extern unsigned char sam_transmit_transparent(unsigned char slotid, unsigned char* apdu, int apdulen, unsigned char* resp, int* resplen);


unsigned char ISO7816_T1_Reset(const unsigned char SamId);
unsigned char ISO7816_T1_Transmit(const unsigned char SamId, unsigned char* ApduCmd, int ApduCmdLength, unsigned char* ApduResp, int* ApduRespLength);



#endif /* ISO7816_H_ */