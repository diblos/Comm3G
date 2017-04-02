/*
 * wifi.h
 *
 * Created: 2012-05-11 5:26:56 PM
 *  Author: Justin
 */ 


#ifndef WIFI_H_
#define WIFI_H_

#define WIFI_ERR_OK						0
#define WIFI_ERR_NETWORK_NOT_FOUND		-1
#define WIFI_ERR_SECKEY_NEEDED			-2
#define WIFI_ERR_CONNECTION_ERR			-3
#define WIFI_ERR_NETWORK_ERR			-4
#define WIFI_ERR_INVALID_IN_PARAM		-5 /*Invalid input parameter*/
#define WIFI_ERR_INVALID_SOCKET			-6
#define WIFI_ERR_TIMEOUT_ERR			-7
#define WIFI_ERR_FAILED					-8
#define WIFI_ERR_WAITING				-10
#define WIFI_ERR_BUSY					-11

#define WIFI_SEC_NONE		1
#define WIFI_SEC_WEP		2
#define WIFI_SEC_WPA_PSK	4
#define WIFI_SEC_WPA2_PSK	8
#define WIFI_SEC_WPA_ENT	16
#define WIFI_SEC_WPA2_ENT	32


extern const char WIFI_STATEXT_NOT_SUPPORT[];
extern const char WIFI_STATEXT_WAIT_HW[];
extern const char WIFI_STATEXT_NOT_CONNECT[];
extern const char WIFI_STATEXT_NO_HW[];
extern const char WIFI_STATEXT_RADIO_OFF[];



typedef struct{/*Text Box*/
	char mac[20];
	char wstate[20];
	char bssid[20];
	char ssid[32];
	char channel[4];
	char security[25];
	char rssi[5];
	char ip[20];
	char subnet[20];
	char gateway[20];
	char dns1[20];
	char dns2[20];
	char rxcount[16];
	char txcount[16];
}atnstat;

typedef struct{
	char bssid[20];
	char ssid[32];
	char channel[3];
	char type[6];
	char rssi[5];
	char security[25];
}atwsitem;

#define SSID_SLOT_MAX_COUNT 30
typedef struct{
	int count;
	atwsitem items[SSID_SLOT_MAX_COUNT];
}atws;

//
// Status
//
// Obtian WIFI Network Status
extern atnstat	wifi_get_nstat(void);

// Obtain SSID detected
extern atws		wifi_get_ws(void);

// Obtain Signal strength of a connected WIFI
extern int		wifi_get_signal_strength(void);

// Obtain WIFI status lable (In General)
extern char*	wifi_get_status_label(void);

// Obtain wifi module firmware version
// out: VersionString. if not required output string, set it to zero
// return: 0-OK, Other-Failed
extern int wifi_get_firmware_version(char* outVersionString);

// WIFI Sync Time - To Sync Wifi Date Time to HammerHead DateTime
// in/out/ - null
// return 0-OK, Other-Failed
extern int wifi_sync_time(void);


//
// Connection
//
extern int wifi_radio_power(unsigned char status);
extern int wifi_connect(char* ssid, char* securitykey,char* auth,char* encryption);
extern int wifi_connect_hidden_ssid(char* ssid, char* securitykey, int securitytype);//connection to hidden SSID, SecurityType must be provided - WIFI_SEC_XXX
extern int wifi_disconnect(void);
extern unsigned char wifi_is_connected(void);//1=connected, 0=notConnected

//
// Configurations
//
extern int wifi_config_static_dns(char* dns1, char* dns2);
// Static IP - mode: 1-DHCP
//				   : 0-Static IP
extern int wifi_config_static_ip(unsigned char mode, char* ip, char* subnet, char* gateway);
extern int wifi_config_save_as_default(void);

//
// DNS
//
//This function to get an IP address from a host name
//in: url - hostname to be identified
//in: param (optional) - 2 bytes. 1st byte indicate retry counter (range from 0 to 10), 2nd byte indicate timeout in second (Range from 0 to 20), set param=0 to use default where retry=0, timeout=2
//out: outIpString - is lookup successful, IP in string will be return here
//return: 0-OK, (-1)-Error. Possible Error: wifi not connected, DNS address not set
int wifi_dns_lookup(char* url, unsigned char* param, char* outIpString);

//
// Access
//
extern int wifi_ping(char* ipdata, char* respdata);

#endif /* WIFI_H_ */