#if defined(__cplusplus)
extern "C"
{
#endif

#ifndef ISO7816_H
#define ISO7816_H

//
// ISO7816
//
#define ISO7816_OFF_CLA			0
#define ISO7816_OFF_INS			1
#define ISO7816_OFF_P1			2
#define ISO7816_OFF_P2			3
#define ISO7816_OFF_LC			4
#define ISO7816_OFF_CDATA		5

//
// SW12
//
#define ISO7816_SW12_OK						0x9000
#define ISO7816_SW12_INS_NOT_SUPPORT		0x6D00
#define ISO7816_SW12_CLA_NOT_SUPPORT		0x6E00
#define ISO7816_SW12_NO_PRECISSE			0x6F00
#define ISO7816_SW12_WRONG_LEN				0x6700
#define ISO7816_SW12_CONDITION				0x6985
#define ISO7816_SW12_INCORRECT_P1P2			0x6A86

//
// CMD
//

//
// EXPORT
//
extern unsigned char ISO7816_APDU[];
extern unsigned char ISO7816_RESP[];
extern int ISO7816_APDU_LENGTH;

extern void iso7816_set_header(unsigned char* DataIn);
extern void iso7816_append_data(unsigned char* DataIn, int DataInLength);
extern void iso7816_set_component(unsigned char ComponentOff, unsigned char Data);
extern void iso7816_set_p1(unsigned char P1);
extern void iso7816_set_p2(unsigned char P2);
extern void iso7816_set_le(unsigned char le);
extern unsigned char iso7816_is_sw12(unsigned char* DataIn, int DataInLength, unsigned short SW12);
extern unsigned char iso7816_is_9000(unsigned char* DataIn, int DataInLength);

#endif

#if defined(__cplusplus)
}
#endif 
