/*
 * pcd.h
 *
 * Created: 2012-03-23 12:27:05 PM
 *  Author: Justin
 */ 


#ifndef PCD_H_
#define PCD_H_

#define MC_SUBSET_HANDLE				1

#define PICCA							0x0001 /* PICC Card Type A*/						
#define PICCB							0x0002 /* PICC Card Type B*/
#define CASLEVEL1						0x93
#define CASLEVEL2						0x95
#define CASLEVEL3						0x97

#define PCD_ER_OK						0x00U /**< Returned in case of no error. */
#define PCD_ER_OK_CHAINING				0x71U /**< Rx chaining is not complete, further action needed. */
#define PCD_ER_OK_INCOMPLETE_BYTE		0x72U /**< An incomplete byte was received. */

/*@}*/

/** \name Communication Errors
*/
/*@{*/
#define PCD_ER_IO_TIMEOUT               0x01U /**< No reply received, e.g. PICC removal. */
#define PCD_ER_INTEGRITY_ERROR          0x02U /**< Wrong CRC or parity detected. */
#define PCD_ER_COLLISION_ERROR          0x03U /**< A collision occured. */
#define PCD_ER_BUFFER_OVERFLOW          0x04U /**< Attempt to write beyond buffer size. */
#define PCD_ER_FRAMING_ERROR            0x05U /**< Invalid frame format. */
#define PCD_ER_PROTOCOL_ERROR           0x06U /**< Received response violates protocol. */
#define PCD_ER_AUTH_ERROR               0x07U /**< Authentication error. */
#define PCD_ER_READ_WRITE_ERROR         0x08U /**< A Read or Write error occured in RAM/ROM or Flash. */
#define PCD_ER_TEMPERATURE_ERROR        0x09U /**< The RC sensors signal overheating. */
#define PCD_ER_RF_ERROR                 0x0AU /**< Error on RF-Interface. */
#define PCD_ER_INTERFACE_ERROR          0x0BU /**< An error occured in RC communication. */
#define PCD_ER_LENGTH_ERROR             0x0CU /**< A length error occured. */
#define PCD_ER_POWER_SLEEP				0x1FU /** PCD Current in Sleep Mode. Instruction not allowed, PCD WAKE UP shall be call to wake up the device first*/
#define PCD_ER_INTERNAL_ERROR           0x7FU /**< An internal error occured. */
//MAP
#define PCD_ER_NOTAGERR					PCD_ER_IO_TIMEOUT
#define PCD_ER_SERNRERR					PCD_ER_INTEGRITY_ERROR
#define PCD_ER_BITCOUNTERR				PCD_ER_INTEGRITY_ERROR
#define PCD_ER_PARITYERR				PCD_ER_INTEGRITY_ERROR
#define PCD_ER_COLLERR					PCD_ER_COLLISION_ERROR
#define PCD_ER_CRCERR					PCD_ER_INTEGRITY_ERROR
#define PCD_ER_CHK_FAILED				0xFF
#define PCD_ER_COM_ERR					0xF1
#define PCD_ER_USER_STOPPED				0xF0
#define PCD_ER_INVALID_INPUT			0xEF
/*@}*/

/** \name Parameter & Command Errors
*/
/*@{*/
#define PCD_ER_INVALID_DATA_PARAMS      0x20U /**< Invalid data parameters supplied (layer id check failed). */
#define PCD_ER_INVALID_PARAMETER        0x21U /**< Invalid parameter supplied. */
#define PCD_ER_PARAMETER_OVERFLOW       0x22U /**< Reading/Writing a parameter would produce an overflow. */
#define PCD_ER_UNSUPPORTED_PARAMETER    0x23U /**< Parameter not supported. */
#define PCD_ER_UNSUPPORTED_COMMAND      0x24U /**< Command not supported. */
#define PCD_ER_USE_CONDITION            0x25U /**< Condition of use not satisfied. */
#define PCD_ER_KEY                      0x26U /**< A key error occured. */

// PCD EEPROM ADDRESSING
#define PCD_EEA_PRODUCT_NAME			0x0200
#define PCD_EEA_PRODUCT_SN				0x0210
#define PCD_EEA_MAIN_BOARD_V			0x0220
#define PCD_EEA_ANT_BOARD_V				0x0224
/*		
		0x00C0	to	0x01FF			RFU
0x0200	to	0x020F	16		Product Name
0x0210	to	0x021F	16		Product Serial Number
0x0220	to	0x0223	4		Main Board Version
0x0224	to	0x0227	4		Antenna Version
0x0228	to	0x022B	4		Touch Version
*/

//extern void pcd_reset(void);
extern void test_pcd(void);

extern void pcd_reset(void);/*hardware reset*/
extern unsigned char pcd_init(void);/*hardware initialization. Return 0=OK, 0xFF=Failed*/
extern unsigned char pcd_get_id(unsigned char* productinfo8);//obtain pcd hardwar info

extern unsigned char pcd_applyprotocol(unsigned char bCardType);
extern unsigned char pcd_rf_off(void);
extern unsigned char pcd_rf_on(void);
extern int pcd_is_rf_on(void);
extern unsigned char pcd_rf_reset(void);
extern unsigned char pcd_reqa(unsigned char* atqa);
extern unsigned char pcd_wupa(unsigned char* atqa);
extern unsigned char pcd_anticol(unsigned char level, unsigned char* uid);
extern unsigned char pcd_select(unsigned char level, unsigned char* uid, unsigned char* sak);
extern unsigned char pcd_hlta(void);
extern unsigned char pcd_rats(unsigned char parameter, unsigned char* ats, int* atslen);
extern unsigned char pcd_activate_a(void);
extern unsigned char pcd_activate_b(void);

extern unsigned char pcd_mi_auth(unsigned char blockno, unsigned char keytype, unsigned char* key);
extern unsigned char pcd_mi_read(unsigned char blockno, unsigned char* Resp, int* RespLen);
extern unsigned char pcd_mi_write(unsigned char blockno, unsigned char* datain);
extern unsigned char pcd_mi_increment(unsigned char blockno, unsigned char* value);
extern unsigned char pcd_mi_decrement(unsigned char blockno, unsigned char* value);
extern unsigned char pcd_mi_retore(unsigned char blockno);
extern unsigned char pcd_mi_transfer(unsigned char blockno);
extern unsigned char pcd_mi_readvalue(unsigned char blockno, unsigned char* value, unsigned char* addrdata);
extern unsigned char pcd_mi_writevalue(unsigned char blockno, unsigned char* value, unsigned char addrdata);
extern unsigned char pcd_mi_incrementtransfer(unsigned char blockno, unsigned char* value);
extern unsigned char pcd_mi_decrementtransfer(unsigned char blockno, unsigned char* value);

extern unsigned char pcd_reqb(unsigned char* resp, int* resplen);
extern unsigned char pcd_wupb(unsigned char* resp, int* resplen);
extern unsigned char pcd_attrib(unsigned char* atqb, int atqblen, unsigned char* resp, int* resplen);

extern unsigned char pcd_settimeout_a(unsigned char fwi, unsigned char swtx);
extern unsigned char pcd_settimeout_b(unsigned char fwi, unsigned char swtx);
extern unsigned char pcd_transa(unsigned char* apdu, int apdulen, unsigned char* resp, int* resplen);
extern unsigned char pcd_transb(unsigned char* apdu, int apdulen, unsigned char* resp, int* resplen);
extern unsigned char pcd_transa_to(unsigned char* apdu, int apdulen, unsigned char* resp, int* resplen, unsigned char fwi);
extern unsigned char pcd_transb_to(unsigned char* apdu, int apdulen, unsigned char* resp, int* resplen, unsigned char fwi);

extern void pcd_debugprint(char* msg, unsigned char* data, int datalen);

//
// PCSC Equivalent API
//
extern unsigned char pcd_sc_list(char* readerlist);
extern unsigned char pcd_sc_connect(void);
extern unsigned char pcd_sc_transmit(unsigned char* apdu, int apdulen, unsigned char* resp, int* resplen);
extern unsigned char pcd_sc_disconnect(void);

#define PCD_SCCONTROL_SET_STOP_POLL		0x01 /*to set a button to stop card polling*/
#define PCD_SCCONTROL_SET_TYPEB_COL		0x02 /*to set TYpe B collision error when encounter protocol error in polling*/
#define PCD_SCCONTROL_ACTIVATION_TYPE	0x03 /*to set the activation type, default is PICCA|PICCB.*/

extern unsigned char pcd_sc_control(unsigned char P1, unsigned char P2, unsigned char* data, int datalen);

//
// PCD Contactless Setting
//
extern unsigned char pcd_get_rc_setting(unsigned char type, unsigned char* sett, int* settlen);
extern unsigned char pcd_set_rc_setting(unsigned char type, unsigned char* sett, int settlen);
extern unsigned char pcd_set_single_rc_setting(unsigned char type, unsigned char address, unsigned char sett);
extern unsigned char pcd_get_single_rc_setting(unsigned char type, unsigned char address, unsigned char *sett);
extern unsigned char pcd_set_tx_wait(unsigned short txwait);


/*
Table 32. TEEPROM memory organization
Section		Page		Byte				Access		Memory content
						addresses			rights
0			0			00 to 31			r			product information and configuration
						32 to 63			r/w

1			1 to 2		64 to 191			r/w			register reset
2			3 to 95		192 to 6143			r/w			free
3			96 to 111	6144 to 7167		w			MIFARE key
4			112 to 128	7168 to 8191		r			Register Set Protocol (RSP)
*/
extern unsigned char pcd_write_ee(unsigned short address, unsigned char* data, unsigned short datalen);
extern unsigned char pcd_read_ee(unsigned short address, unsigned short readlen, unsigned char* data);
extern unsigned char pcd_get_info(unsigned short infonumber, unsigned char* data, int* datalen);
extern unsigned char pcd_set_info(unsigned short infonumber, unsigned char* data, int datalen);


//
// PCD Power Management
//
extern void pcd_sleep(void);
extern void pcd_wake_up(void);


#endif /* PCD_H_ */