/*
 * h2core.h
 *
 * Created: 2012-03-22 5:52:28 PM
 *  Author: Justin
 */ 

#ifndef H2CORE_H_
#define H2CORE_H_

/****************************************************************************************************************	
*H2CORE - VERSIONING
Version		Date		Description
0.1.0.0		-			Beta Release
0.2.0.0		2012-04-08	added PCD function layer
						added Debug Function on h2core
0.2.1.0		2012-04-16	Fixed Generate Random Function (Not Exist)
0.2.2.0		2012-04-17	Bugfixed. Display Driver, Put Pixmap in particular box doesnt show up
0.2.3.0		2012-04-17	BugFixed. PCD SC Connect, return in proper error code. 0=no_card, 1=one_card, 2=more_card, 0xFF=protocol_error
0.2.3.1		2012-04-25	Changes: USB Set Outgoing in while loop until all data sent
0.2.4.0		2012-04-25	BugFixed: USB outgoing always collect full 64 bytes then only sent out. Now fixed with force sent out
0.2.5.0		2012-04-27	Enhanced: added direct access usb-serial outgoing buffer
0.2.6.0		2012-04-28	BugFixed: KeyPad GetChar - add control to listen "key Pressed"
0.3.0.0		2012-04-28	FineTuned: Fined tune PCD antenna on type A for hardware version V20
0.3.1.0		2012-04-30	BugFixed: CONFIG_PBB_HZ	has to be 60000000 in order to speed up USB (64 bytes block frame problem)
0.3.2.0		2012-04-30	BugFixed: SAM1 RST PIN Changed
0.4.0.0		2012-05-04	Added: IntFlash -> SDRAM -> IntFlash pointing jump
						Added: h2core_exit_to_main_area() function for app running in RAM to exit and return back to main app
0.4.1.0		2012-05-10	BugFixed: PCD Type B Activation, and Exchange APDU. Now support EMV Type A and B protocol handling
0.4.2.0		2012-05-10	BugFixed: PCD Type B FSDI change from 0 to 8 to support up to 256bytes per frame
0.4.3.0		2012-05-15	BugFixed: File System, read more than 122880 bytes return rubbished. Solved.
0.4.4.0		2012-05-16	BugFixed: File System, list file infinity loop if file full. Solved.
0.5.0.0		2012-05-24	Added: WIFI Handling, & TCP Socket connection (Up to 3 connection only)
0.5.1.0		2012-05-30	FineTune: WIFI connect and disconnect sequence handling faster
0.5.2.0		2012-06-6	Solved: WIFI connect/disconnect/listen/receive data issue. Improved Stability (WIFI AT SPEED = 115200)
0.6.0.0		2012-06-07	Added: fposix - pfgets - to get a line from file
0.6.1.0		2012-06-13	BugFixed: The first SSID in wifi list always not allowed to connect (Index range problem, solved)
0.6.2.0		2012-06-20	FineTune: Change Gainspan comspeed from 115200 to 38400
0.7.0.0		2012-06-21	Added:watchdog timer, function to reset into isp mode
0.8.0.0		2012-06-21	Added: Mifare Functionality (API)
0.8.1.0		2012-06-26	BugFixed: Draw Fill Box - Limit Area locked
0.9.0.0		2012-07-02	Added: Get Restart Count - for core initialization to determine whether to initialize all hardware
						FineTune: WIFI AT initialization - no longer needed to set configuration - it should be done pre-production
0.9.1.0		2012-07-03	BugFixed: Set Static DNS1, DNS2, software error where it send at+dnsset DNS2, DNS2. Fixed to DNS1 and DNS2
						Changes: LED inited and all LED turned off during startup
0.A.0.0		2012-07-09	BugFixed: WIFI interrupt driver returned. 
									Send Buffer Frame Handling
0.B.0.0		2012-07-10	Added: Support to start run from RAM (Joey)
0.B.1.0		2012-07-12	BugFixed: Added antiloop_count in uart_usb_flush() to prevent deadloop during CDC in STAC and MAC
						Changes: USB Init enable for both flash and ram app
0.C.0.0		2012-07-13	Tunning: Key Pad Value taken by sampling 5 samples with 1sec interval. (Previously 2 samples with no interval)
0.D.0.0		2012-07-25	Added: Battery Metering, Charing status
0.D.1.0		2012-07-25	FineTune: Beep now in non-blocking mode				
0.D.2.0		2012-07-26	BugFixed: Beep no stopped in between jumping flash-ram app. added beep_wait_stop at stop critical service
0.D.3.0		2012-07-26	FineTune: Vibrate now in non-blocking mode
						updated: Color.h (Include more standard color code, i.e. maroon, navy, purple...)
0.D.4.0		2012-08-01	BugFixed: Added SAM connect/transmit/disconnect functions for individual SAM (For HW Version V31)
0.E.0.0		2012-08-23	Added: Hardware callback function to framework (to notify something happen, for status bar update, and cancel card reading purpse)
						Added: Power Management functions = sys_wake_up(), sys_sleep()
0.E.1.0		2012-08-30	BugFixed: Added sys_wake_up() in sys_touch().
0.E.2.0		2012-08-30	Adjustment: BAT MAX 40100, MIN 36000
0.E.3.0		2012-09-03	Enhanced: PCD SC Get Status
0.F.0.0		2012-09-14	Added: Power Management Control (PM & SLEEP)
0.10.0.0	2012-10-02	Changes: Battery meter update according to hardware version WFRD_V32 & WFRD_TOUCH_V32

0.10.1.0	2012-10-05	Singapore MC-Subset testing tuning	
0.10.2.0				Solved PCD Chaining Problem
0.11.0.0	2012-10-17	Added: initilize PCD antenna setting according to version - pcd_configure_ant_version()
0.11.1.0	2012-10-19	wifi addeds at wractive  to 0 when sleep, and 1 when wakeup		
						bugfixed: File System Bad Sector Check now added
0.12.0.0	2012-10-20	Added WIFI ON/OFF function, and handling
0.12.1.0	2012-10-20	Added WIFI Chane module com badurate
0.13.0.0	2012-10-23	Added Magnetic Switch Function
0.13.1.0	2012-10-23	Added Keypulse to make shutdown signal to touch pad
0.13.2.0				BugFixed: LED Sleep/Wake up bug
0.13.3.0	2012-10-24	Remove KeyPad long & Short press. Replaced by sys long & short press
0.14.0.0	2012-10-30	Added: LED Blinking
0.14.1.0	2012-10-31	Added: Loopback Polling for EMV L1 Test
0.15.0.0	2012-11-20	Passed: EMVCO Level 1 Digital Pre-Test (Remove some L1 debug message to other HWID)
0.15.1.0	2012-11-21	Changes: Enable WIFI module (Remove disabling WIFI due to L1 debug test)
						added: wifi_disconnect(..) to disassociate current connected SSID
0.15.2.0	2012-11-21	Changes: Flush TCP connection at startup of WIFI task (in case previously connected)
0.15.3.0	2012-11-26	Changes: PCD activation looping change from 5000m to 100ms
0.15.4.0	2012-11-26	BugFixed: PCD Mifare Read Value not returning correct addrData (Fixed)
0.16.0.0	2012-11-27	BugFixed: lcd_draw_rectfill, added with end limit to avoid blackdraw
						Added: SAM Auto Handling of SW1-0x61, get response request 
0.16.1.0	2012-11-27	BugFixed: SAM. Sam take out and put back, the received data gone crazy (Solved)
0.17.0.0	2012-11-28	Changes. Font bitmap removed from this h2core, and recreate in lib_font.a
0.17.1.0	2012-11-30	[IMPORTANT] Bugfixed: SDRAM wake up not back to normal rate. caused PCD read card problem (Solved) 
0.17.2.0	2012-12-04	[IMPORTANT] BugFixed: TCP Read not doing receiving task
0.17.3.0	2012-12-21	Enhanced: TimerTick interrupt turn off during read card						
						SAM Receiver Char Guard time changed from 100ms to 10ms
0.18.0.0	2012-12-26	Added: FileSystem SetCharacter function
						Enhanced: Battery warning on 20% and 10%
						Enhanced: Wake Up service when battery goes low, and Turn OFF power service although hammerhead in sleep mode
						Enhanced: Timer interrupt turned OFF when PCD is transmitting and receiving
0.18.1.0	2012-12-27	Bugfixed: Run App in RAM sometime hang due to interrupt not closed. Now, added disable global interrupt in disable critical service function
0.18.2.0	2012-12-27	Enhanced: Added WIFI TCP Connect with non-blocking mode
0.18.3.0	2012-12-28	Bugfixed: WIFI recovery with "ATA" not successful sometime. NOw added at+wa to enhance the recovery
0.18.4.0	2012-12-29	FineTuned: SPI baudrate to RC663 changed from 5M to 20M (To Fixing Missing Byte problem)
0.18.5.0	2013-01-08	Bugfixed: WIFI Task prevented from going in waiting stage.
						Bugfixed: LCD get string size. filtered '\n' 
0.18.6.0	2013-01-09	Bugfixed: sleep walker kill the MCU from wakeup, always sleep second time died. (Solved by added wait)
0.18.7.0	2013-01-09	Bugfixed: WIFI task check the wrong WIFI_OFF Flag
0.19.0.0	2013-01-12	Added: Serial Driver for communication
0.19.1.0	2013-01-12	BugFixed: Serial incoming autoflush problem (Solved)
						Enhanced: Card Polling loopcount reduced to 10 (From 50)
0.19.2.0	2013-01-30	Changes: PCD EMV return Multicard, and WTX exit on 8 continuous retry
0.20.0.0	2013-02-01	Bugfixed: Calendar 32nd day fixed
0.20.1.0	2013-02-02	Enhanced: PCD SWTX retry changed from 8 to 20. Wave2 card needs more than 53 SWTX
0.20.2.0	2013-02-04	Enhanced: PCD SWTX retry changed from 20 to 70. Wave2 card needs more than 53 SWTX
0.20.3.0	2013-02-04	Bugfixed: USART Change Baudrate (Returning rubbishing during changing baudrate. Solved)
0.20.4.0	2013-02-05	Bugfixed: USART change baudrate on next cycle need sleep 50ms before changing the clock
0.20.5.0	2013-02-05	Enhanced: sys_set_datetime(..) added check datetime policy, return -1 value for wrong datetime, 0 for success
0.20.6.0	2013-02-19	Enhanced: TickCounter IRQ disable in PCD comm is removed
0.20.7.0	2013-02-22	FineTuned: SPI baudrate to RC663 changed from 5M to 20M (To Fixing Missing Byte problem)
0.20.7.1				Test Release for SPI 5M to test new board SPI noise
0.20.7.2				Added USB Disable in Stop Critical Service
0.21.0.0	2013-02-06	Added: Keypad Enable and Disable
1.0.0.0		2013-03-13	Stable Release: Solve h2core init random hang problem
						Added: Status display for app launching/closing
1.1.0.0		2013-03-25	Added: pcd_sc_control(..) to allow to set Type B Collision Detection on Protocol Error
1.1.1.0		2013-03-31	Enhance: added repeat WUPB to check collision on type B card
1.1.2.0		2013-04-12	Enhance: Added ATTRIB, then following by WUPB to check collision on type B
1.2.0.0		2013-04-22	Updated: updated LCD driver to support New (V 0xE3) LCD Module
						Changes: PCD RxAna for Type B set to 0x0A
						Added: USB Off On Function
1.3.0.0		2013-04-25	Added: USB Handler to support Ingenico Terminal for App running in RAM
						Changes: Battery Low level changed from 36000 to 36500
1.3.1					Rebuilt with enabling wifi buffer for app usage
1.3.2		2013-05-03	Battery Level on USB change from 5000 to 9000
						BugFixed: App startup in up side down (now support the new LCD)
1.3.3		2013-05-07	Enhance: Type B Collision Check tuning for Singapore Ezlink Card
1.4.0.0		2013-05-08	Added: SC Control to enable only Type A or Type B
						Enhance: Type Collision to use Slot Marker
			---
1.5.0.0		2013-05-16	Enhance: Change to 6M SPI
						Added: Restart Function
						Added: Watch Dog for App Startup (Will be restarted if app failed during startup)
1.6.0.0		2013-05-22	Changes: WIFI Initialized with disable ECHO, and set BDATA = 1.
						Enhance: Hidden File Support
						Added: Background USB task in PCD polling
1.6.1.0		2013-06-03	BugFixed: SW1=0x61 handling added for SAM transmit short frame (5 bytes APDU)
						BugFixed: SAM Connect: Continue to Read during power up in case first receive is only one byte
						Enhance: Read PCD response after FIFO is finished read
						//Change SWTX handle to 500 count			
1.7..0.0				Enhance: USB data push out immediately on usb_serail_set_outgoing(...)
						added: com/usb incoming listening during PCD SWTX handling for Paypas 3.0 stop signal handling
1.7.1.0.0				Changes: use PCD_SC_CONTROL to turn ON/OFF com/usb incoming listening during PCD SWTX handling for Paypas 3.0 stop signal handling
1.7.2						Optimize Speed for pp performance
1.7.3					Enhance: FS auto init after format
1.7.3.1					Enhance for Paypass Performance Test
1.7.4.0		2013-08-01	Bugfixed: First time format do not required to restart again
1.7.5.0		2013-08-06	BugFixed: USB Hang during USB debug (Connected to PC visa USB, but COM not opened)
						Enhanced: Battery Meter during charging adjusted according to non-charge curve.
1.7.6.0		2013-08-09	Bugfixed: PM hang (1% from mass-production) - adjusted crystal constant
1.7.7.0		2013-08-12	Bugfixed: " Second Fixed
1.8.0.0		2013-08-12	Increase LCD Reset Time from 20msec to 120msec (White Screen during startup)
						increase LCD reset to 200ms
1.9.0.0		2013-08-22	Added: ScControl to use REQB for activation - EzLink requirement
1.10.0.0	2013-08-22	Added: WIFI SSID Slot increased from 16 to 32;
						Added: WIFI QA TEST
1.10.1.0	2013-08-23	BugFixed: WIFI SSID LISTING more than 24 will hang. Now support up to 32. 
1.10.2.0	2013-09-04	BugFixed: pfgets will be reseted to 1st line by pfopen and pfclose. Previously still keep the line number
1.11.0.0	2013-09-13	Added: PCD low level function to support SG-EZLINK Hybrid Card Polling
1.11.1.0	2013-09-13	FinedTune: Startup Watchdog changed from 3sec to 6sec
1.12.0.0	2013-09-18	Added: pfgetfilecount - to get total file in disk, and total bytes used
						Added: File Extension ".ink" is hidden file
1.12.1.0	2013-09-19	Bugfixed: Format Disk Bug. Erase page required to convert block to page (in format function) find "1.12.1.0"
1.13.0.0	2013-09-26	Added: WIFI SSL Connection
						Added: WIFI Connect Hidden SSID function
1.13.1.0	2013-09-27	Added more debug message in WIFI TCP READ
1.14.0.0	2013-11-12	Added: LCD Rotate - THis function only allowed in App (RAM Sector)
1.14.1.0	2013-11-13	BugFixed: LCD draw icon function to put bitmap required to add avoid black function
1.15.1.0	2013-11-26	Added: reset-watchdog timer function
1.15.2.0	2013-12-09	BugFixed: reset-watchdog not updating watchdog timer
1.16.0.0	2013-12-09	Added: Support Connect to WIFI without DHCP (IP COnfig Error)
1.17.0.0	2013-12-10	Added: Library Object Type Checking. Ocean or App type (Warning on status bar is not correct)
1.18.0.0	2013-12-30	Added: PM Initialization check. If PM not initialized correctly. restart will be called
1.18.1.0	2014-01-07	Bugfixed: Added Antiloop conuter check on USB handshake cdc_set_control_line_state(), cdc_set_line_coding(), cdc_get_line_coding()
1.19.0.0	2014-01-22	Added: File System function to get file count according to file header and extension
1.20.0.0	2013-02-24	BugFixed:IMPORTANT: fix USB problem on iAntiloop for standard usb driver. iAntiloop only open for VERIX
1.20.1.0	2013-02-26	BugFixed: Beep_wait_finish(...) to remove closing buzzer TC. It will be handle by interrupt. double handle will causing hang
1.20.2.0	2014-04-10	Enhanced: added more loop to receive SAM ATR in sam_connect function
1.20.3.0	2014-04-22	Enhanced: Added lcd rotate by angle: void tm023kdz_setmirror_angle(unsigned char x, unsigned char y, unsigned char exchange)
1.21.0.0	2014-05-29	Enhanced: USB Serial Write (change to putpacket 63bytes one short instead of putchar 1byte)
1.22.0.0	2014-06-27	Enhanced: Touch sensitivity sampling adjustment during charging
						getkey input different in charging and not charging
1.23.0.0	2014-08-19	Added: SAM support T=1 protocol
						Added: SSL Open to allow to open without needed any cert
						Added: DNS Look Up (wifi.h)
1.24.0.0	2014-08-21	Enhance: Added function for SSL connection in non-blocking mode
1.24.1.0	2014-09-02	BugFixed: Lcd_DrawPixel cannot draw on pixel after position 256. solved
1.25.0.0	2014-09-10	Enhanced: Support Mifare Classis 7Bytes UID on 3PASS Auth
1.26.0.0	2014-09-17	Added: Support T=1 Protocol, at 9600 default baudrate only
						Enhance, WIFI stream buffer changed from 3000 to 5000 bytes

2.0.0.0					New Phase h2core
						- Yaffs
						- 3G/WIFI/MN8
2.1.1.0					Set DHCP failed (SG NETS) 
2.1.2.0					"
2.1.3.0					WIFI set static IP and DNS Failed. Due to wrong alloc memory. Solved
2.1.4.0					PCD Initialization problem. causing PCD not functioning. Solved (by removed LITE_VERSION=1)
2.1.5.0					Enlarge Watchdog timer during yaffs init, to 20sec. and back in 6sec after
2.2.0.0					Added: Folder access by SCPI pf_lsall(...)
2.2.1.0					BugFixed: filename size in pfopen for yaffs enlarged from 64chars to 512chars 

-- 
						WIFI disconnect wrong error code
						Debug build 
2.3.0.0		2015-03-19	Added: NonBlocking function for WIFI connect, WIFI Disconnect, WIFI TCP Disconnect
2.4.0.0		2015-04-03	BugFixed: RTC day of month not incremented corrected. i.e. Mar 30 will jump to Apr 1, skipping Mar 31. Solved
2.4.1.0		2015-04-06	BugFixed: Added ttl_init in core_init for ocean. else exitToMain will hang
						Adjustment: remove error check in AT&Y0 set default profile. GS firmware version 2.5.9 return error in this request
2.4.2.0		2015-04-10	Adjustment: Change SAM numbering for V2. swap 1 and 2, swap 3 & 4
						Added: QA function for 3G and MN8
2.4.3.0		2015-05-04	BugFixed: Adapting Gainspan FW 2.5.9, TCP connect always increment CID. gswifi handle crop for CID
2.4.4.0		2015-05-05	BugFixed: miss handle crop cid, 1) cropcid(...), 2) flushTcp(...)
2.5.0.0		2015-05-05	Enhancement from h2core V1:
						1) support PPS
						2) frame waiting time set to 1000ms
						3) support vix t1 sam. added reset_iso7816 settings in one write
2.5.1.0		2015-05-10	Bugfixed & Enhance: Added healthy check for gainspan module, if module not respoding (always timeout), it will be reset and restart
X.X.X.X		2015-05-14	BugFixed: pfread with input size=0, required to read from offset until end of file
2.5.2.0		2015-05-26	BugFixed: heathy check and recover to include DHCP, Static IPs, and DNS
2.5.3.0		2015-05-26	BugFixed: addded timeout control for wifi disconnect non blocking. if timeout and no resp, assumed ok
2.5.4.0		2015-05-27	Enhancement: Change non-blocking get status read timeout from 30ms to 3ms
2.5.5.0		2015-05-28	Added sync time before ssl open (blocking and non-blocking)
2.5.6.0		2015-06-03	Enhancement: remove ATA recovery in SGNETS version
2.5.6.1					Enhancement: remove NSTAT call after wifi connect, config dhcp, config static ip
2.5.7.0		2015-06-04	Bugfixed: DHCP Config Error Infinite loop - solved.
2.6.0.0		2015-06-12	added battery level & new calculation formula for V2
2.7.0.0		2015-09-28	added speaker function
						
*****************************************************************************************************************
V2 Start
- 4 SAM
- Buzzer
- 3G/WIFI
*****************************************************************************************************************
2.0
					
*****************************************************************************************************************/
#define H2CORE_VERSION		0x02060000
#define H2CORE_RDATE		0x20150610
#define H2CORE_HW_VERSION	0x00033100

//#define H2CORE_FOR_OCEAN

/****************************************************************************************************************
*	
*H2CORE - CORE FUNCTIONS
*
*****************************************************************************************************************/

// Hardware Bitmap
// The Hardware ID bitmap. Returned by h2core_init(),
// If the return value is masked to any bit below, that's mean the initialization of that specify hardware is failed.
#define H2HW_MCU			(1<<0)
#define H2HW_RTC			(1<<1)
#define H2HW_SERIAL			(1<<2)
#define M2HW_USB			(1<<3)
#define H2HW_BATT			(1<<4)
#define H2HW_LCD			(1<<5)
#define H2HW_CLOCK			(1<<6)
#define H2HW_BUZZER			(1<<7)
#define H2HW_LED			(1<<8)
#define H2HW_KEYPAD			(1<<9)
#define H2HW_SAM			(1<<10)
#define H2HW_PCD			(1<<11)
#define H2HW_USB			(1<<12)
#define H2HW_FS				(1<<13)
#define H2HW_WIFI			(1<<14)
#define H2HW_RAM			(1<<15)
#define H2HW_MSWITCH		(1<<16)
#define H2HW_VIBRATE		(1<<17)
#define H2HW_3G				(1<<18)
#define H2HW_TTL			(1<<19)

#define H2HW_CORE_PROCESS	(1<<20)
#define H2HW_PCDREG			(1<<21)

#define H2HW_DISABLE_SERIAL_DEBUG	(1<<22)
#define H2HW_ENABLE_USB_DEBUG		(1<<23)

#define H2HW_TCP					(1<<24)

// h2core_init(), the first command to be called in main(...) for hardware initialization
// in:	ignorehardware - the HWID of hardware that not required to initialize. Recommended to use 0
// ret:	HWID that not successfully initialized*/
extern unsigned int h2core_init(unsigned int ignorehardware);

//h2core_task(), this function SHALL be placed in main()'s while loop's first position, in order to feed the sequential process of h2core
extern int h2core_task(void);

//h2core_close(), this function terminate/cloase all hardware services.
extern int h2core_close(void);

//h2core_exit_to_main_sector() - exit current app in sdram, and set the application pointer back to main app position (0x0)
extern void h2core_exit_to_main_sector(void);

//Restart Device
void h2core_soft_restart(void);

//Reset Watchdog timer
// Recommended to have max 20sec for startup time only
void h2core_reset_watchdog_timer(int timeoutms);

//register a void fuction in h2core, which h2core_task will be calling 
void h2core_task_register_callback(void* callbackfx);

//h2core_get_version() - obtain the h2core lib version
extern unsigned int h2core_get_hw_name(char** name);
extern unsigned int h2core_get_hw_version(void);
extern unsigned int h2core_get_hw_id1(void);
extern unsigned int h2core_get_hw_id2(void);
extern unsigned int h2core_get_volume_id(void);
extern unsigned int h2core_get_version(void);

// h2core_get_running_sector() - to check the sector that currently run by the app
#define SECTOR_UNKNOWN		0
#define SECTOR_BOOT			1
#define SECTOR_FLASH		2
#define SECTOR_STORAGE		3
#define SECTOR_RAM1			4
#define SECTOR_RAM2			5
extern unsigned char h2core_get_running_sector(void);



/****************************************************************************************************************
*	
*H2CORE - SYSTEM FUNCTIONS
*
*****************************************************************************************************************/

// gettickcount(), obtain the system count in ms. The system start (first power up) at 0
// ret:	system tick
extern unsigned long gettickcount(void);

// sleep(), the delay function
extern void sleep(unsigned long delay);//delay.h

//Generate Random Number
// In:	len, size of random number to be generated, in byte count
// out:	destbuf, buf to store random number
extern void generate_random(int len, unsigned char* destbuf);

// sys_get_datetime(), obtain system datetime, format in YYMMDDHHmmSSWD
// out:	datetime7, the pointer to store date time
// ret: Length of date time(7), Negative indicate error
extern int sys_get_datetime(unsigned char* datetime7);//rtc=isl1208.h

// sys_set_datetime(), update system datetime, format in YYMMDDHHmmSSWD
// out:	datetime7, the pointer to store date time
// ret: Length of date time(7), Negative indicate error
extern int sys_set_datetime(unsigned char* datetime7);//rtc=isl1208.h

//sys_touch(), make a touch to device. to notify "it is something happen"
extern void sys_touch(void);

/****************************************************************************************************************
*	
*H2CORE - LED FUNCTIONS (extended from led.h)
*
*****************************************************************************************************************/

// LED ID
#define LED_1				0x01//91/*AVR32_PIN_PX40*/
#define LED_2				0x02//43/*AVR32_PIN_PB11*/
#define LED_3				0x04//31/*AVR32_PIN_PA31*/
#define LED_4				0x08//27/*AVR32_PIN_PA27*/

// ledoff() - Turn OFF the specified led (To turn off 2 or more leds together, use OR, e.g. LED_1|LED_2)
extern void led_off(unsigned char leds);

// ledon () - Turn ON the specified led
extern void led_on(unsigned char leds);

//led blink
extern void led_blink(unsigned char leds, int onintervalms, int offintervalms);//stop interval = 0


/****************************************************************************************************************
*	
*H2CORE - BUZZER FUNCTIONS (extended from buzzer.h)
*
*****************************************************************************************************************/

// beep() - make a beep sound
// in:	freq - beep frequency
// in:	periodms - the period of beeping in milliseconds
extern void beep(int freq, int periodms);

// beepstd() - make a standard beep (std freq=2700 ,periodms=300)
extern void beepstd(void);

//wait until the non-blocking mode beeping finish
extern void beep_wait_finish(void);

extern void sound_negative(void);
extern void sound_notify(void);
extern void sound_positive(void);


/****************************************************************************************************************
*	
*H2CORE - HACTITLE/Vibrate FUNCTIONS (extended from vibrator.h)
*
*****************************************************************************************************************/

// vibrate() - make a hactile
// in:	periodms - vibration period in milliseconds
extern void vibrate(int periodms);


/****************************************************************************************************************
*	
* H2CORE - LCD FUNCTIONS (extended from lcd.h)
*
*****************************************************************************************************************/

// LCD Info
#define LCD_WIDTH	320
#define LCD_HEIGHT	240

// lcd_draw_pixel() - draw a single pixel onto LCD
// in:	x - the x position of pixel
// in:	y - the y position of pixel
// in:	color - the color code of pixel (color.h)
extern void lcd_draw_pixel(unsigned short x, unsigned short y, unsigned short color);

// lcd_draw_string() - draw a string text onto LCD
// in:	lcd_string - pointer to string text
// in:	font_style - type of the font (font.h)
// in:	x - the x position of string text
// in:	y - the y position of string text
// in:	fcolor - forecolor (color.h)
// in:	bcolor - backgound color (color.h)
extern void lcd_draw_string(char *lcd_string, const unsigned char *font_style, unsigned short x, unsigned short y, unsigned short fcolor, int bcolor);

// lcd_draw_fillrect() - draw fill rectangle
// in:	x - the x position
// in:	y - the y position
// in:	width - the width of object
// in:	Height - the height of object
// in:	color - the fill color
extern void lcd_draw_fillrect( unsigned short x, unsigned short y, unsigned short width, unsigned short height, unsigned short color);

// lcd_draw_horizline() - draw a horizontal line
// in:	x - the x position
// in:	y - the y position
// in: length - the length of line to be draw
// in: color - color of line
extern void lcd_draw_horizline( unsigned short x, unsigned short y, unsigned short length, unsigned short color);

// lcd_draw_vertline() - draw a vertical line
// in:	x - the x position
// in:	y - the y position
// in: length - the length of line to be draw
// in: color - color of line
extern void lcd_draw_vertline( unsigned short x, unsigned short y, unsigned short length, unsigned short color);

// lcd_draw_line() - draw a line
// in:	x1 - the x position of line start
// in:	y1 - the y position of line start
// in:	x2 - the x position of line end
// in:	y2 - the y position of line end
// in: color - color of line
extern void lcd_draw_line( unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned short color );


extern void lcd_draw_bitmap(unsigned char *bitmap,  unsigned short map_width,  unsigned short x,  unsigned short y,  unsigned short width,  unsigned short height,  unsigned short foreground_color,  unsigned short background_color);

// lcd_draw_pixmap() - draw pixmap picture (16-bits)
// in:	pixmap - pointer to pixmap location
// in:	map_width - width of pixmap
// in:	map_heigth - height of pixmap
// in:	map_x - the x position of pixmap to be draw
// in:	map_y - the y position of pixmap to be draw
// in:	x - the x position on LCD to be draw
// in:	y - the y position on LCD to be draw
// in:	width - the width to be draw
// in:	height - the height to be draw
extern void lcd_draw_pixmap(unsigned short const *pixmap, unsigned short map_width, unsigned short map_x, unsigned short map_y, unsigned short x, unsigned short y, unsigned short width, unsigned short height);

// lcd_draw_gcolorimage
// gColorImage arrage - first 4 bytes are image attribute {0x1010, width, height, 0x010B, ... following by 16bits color depth data ...}
extern void lcd_draw_gcolorimage(const unsigned short* gcolorimgae, unsigned short x, unsigned short y, unsigned short width, unsigned short height);


extern void lcd_draw_xbar(unsigned short* barmap, unsigned short x, unsigned short y, unsigned short width, unsigned short height);
extern void lcd_draw_icon(const unsigned char* bitmapicon, unsigned short x, unsigned short y, unsigned short fcolor, int bcolor);
extern void lcd_draw_icon_xbar(const unsigned char* bitmapicon, unsigned short x, unsigned short y, unsigned short* xbarcolor);



/****************************************************************************************************************
*	
*H2CORE - KEYPAD FUNCTIONS (extended from keypad.h)
*
*****************************************************************************************************************/
#define KEY_ID_NULL		0xFF
#define KEY_ID_N0		0x00
#define KEY_ID_N1		0x01
#define KEY_ID_N2		0x02
#define KEY_ID_N3		0x03
#define KEY_ID_N4		0x04
#define KEY_ID_N5		0x05
#define KEY_ID_N6		0x06
#define KEY_ID_N7		0x07
#define KEY_ID_N8		0x08
#define KEY_ID_N9		0x09
#define KEY_ID_ASTR		0x0A
#define KEY_ID_HASH		0x0B
#define KEY_ID_BACK		0x0C
#define KEY_ID_ENTER	0x0D
#define KEY_ID_F1		0x0E
#define KEY_ID_F2		0x0F
#define KEY_ID_F3		0x10
#define KEY_ID_F4		0x11
#define KEY_ID_POW0S	27
#define KEY_ID_POW4S	28
#define KEY_ID_POW2S	29
#define KEY_ID_POW1S	30

#define KEY_ID_SLEEP	0x30
#define KEY_ID_POWIFY	0x29	/*Power Notification*/

// keypad_getinput() - obtain user input on keypad
extern unsigned char keypad_getinput(void);

//Enable keypad input
extern void keypad_enable(void);

//Disable Keypad Input
extern void keypad_disable(void);


/****************************************************************************************************************
*	
*H2CORE - BATTERY FUNCTIONS (extended from battery.h)
*
*****************************************************************************************************************/
#define BAT_STAT_PRECHARGE		0x00
#define BAT_STAT_FASTCHARGE		0x01
#define BAT_STAT_CHARGECOMPLETE	0x02
#define BAT_STAT_CHARGESUSPEND	0x03

#define BAT_CALLBACK_LOW_POW	0x01 /*Required Shutdown*/
#define BAT_CALLBACK_LOW_10		0x02
#define BAT_CALLBACK_LOW_20		0x03
#define BAT_CALLBACK_CHARGE_FUL	0x10

// battery_get_percentage() - Retrieve battery juice in percentage
extern int battery_get_percentage(void);

// battery_get_voltage() - Retrieve battery Voltage Level
extern int battery_get_voltage(void);

// battery_get_charging_status() - Obtain Charging Status
extern unsigned char battery_get_charging_status(void);

//register call back function for battery status notification callbabck
extern void battery_register_callback(void* callbackfx);


/****************************************************************************************************************
*	
*H2CORE - SERIAL PORT FUNCTIONS (Console Port)
*
*****************************************************************************************************************/

// serial_print() - print a string to serial port (Console)
// in:	string - pointer to message string
extern void serial_print(const char *string);


/****************************************************************************************************************
*	
* H2CORE - USB CDC FUNCTIONS (Serial Emulator)
* USB CDC is the main communication port of device
*
*****************************************************************************************************************/
// usb_serial_is_connected() - Return the status whether USB has been connected (CDC Mode) succesfully
// ret:	1=Connected, 0=Not Connected
extern unsigned char usb_serial_is_connected(void);

// usb_serial_get_incoming() - listen to incoming message (at USB port)
// out:	buf - pointer to buffer pointer, that storing the incoming messages
// ret:	Length of incoming message
extern int usb_serial_get_incoming(unsigned char** buf);

// usb_serial_flush_incoming() - to flush incmoing message (at USB port)
// recommended to flush the processed data (For example, if an app process only 10 bytes from incoming, but there are 15 bytes. The app SHALL only flush 10 bytes
// in:	len - len of incoming buffer to be flushed
// ret:	The flushed size
extern int usb_serial_flush_incoming(int len);

// usb_serial_set_outgoing - to return data (response) from device to external device via USB
// in: data - pointer to data buffer that to be sent out
// in: datalen - length of data
// ret: Total data buffer that being queue at outgoing buffer to be sent
extern int usb_serial_set_outgoing(unsigned char* data, int datalen);

// usb_serial_outgoing_buffer_pointer() - obtain the free buffer pointer to put outgoing data, it required to follow by calling "usb_serial_set_outgoing_length()" in order to push the data out from usb serial effectively
// note: This function and usb_serial_set_outgoing_length() are used together to be equal to usb_serial_set_outgoing()
// ret: the free buf pointer to store response data
extern unsigned char* usb_serial_outgoing_buffer_pointer(void);

// usb_serial_set_outgoing_length() - commit the outgoing data from "usb_serial_outgoing_buffer_pointer()"
// note: This function and usb_serial_outgoing_buffer_pointer() are used together to be equal to usb_serial_set_outgoing()
// ret: Total data buffer that being queue at outgoing buffer to be sent
extern int usb_serial_set_outgoing_length(int datalen);

/****************************************************************************************************************
*	
* H2CORE - UART SERIAL FUNCTIONS 
* UART Serial is the second communication port of device
*
*****************************************************************************************************************/
// uart_serial_change_badurate() - Change baudrate
// Baudrate: 19200. 38400, 57600, 115200
// Mode: Immedite=1, Next Cycle=0 (Next cycle change baudrate effective after a response on UART completed)
// ret:	0-OK
extern unsigned char rs232_serial_set_baudrate(unsigned long baudrate, unsigned char mode);
extern unsigned long rs232_serial_get_baudrate(void);

// usb_serial_get_incoming() - listen to incoming message (at USB port)
// out:	buf - pointer to buffer pointer, that storing the incoming messages
// ret:	Length of incoming message
extern int rs232_serial_get_incoming(unsigned char** buf);

// usb_serial_flush_incoming() - to flush incmoing message (at USB port)
// recommended to flush the processed data (For example, if an app process only 10 bytes from incoming, but there are 15 bytes. The app SHALL only flush 10 bytes
// in:	len - len of incoming buffer to be flushed
// ret:	The flushed size
extern int rs232_serial_flush_incoming(int len);

// usb_serial_set_outgoing - to return data (response) from device to external device via USB
// in: data - pointer to data buffer that to be sent out
// in: datalen - length of data
// ret: Total data buffer that being queue at outgoing buffer to be sent
extern int rs232_serial_set_outgoing(unsigned char* data, int datalen);


/****************************************************************************************************************
*	
*H2CORE - SOCKET FUNCTIONS
*
*****************************************************************************************************************/

// wifi_tcp_connect(), to make a connection to server, or to listen to a local port
// in: destip: destination ip in ascii, example, "192.168.1.5". for listening mode, set destip = 0;
// in: port: destination (or local for listening) port number in ascii. example "8090"
// ret:	Positivite Number: Connection ID. CID. Negative Number is Error
extern int wifi_tcp_connect(char* destip, char* port);

// wifi_tcp_disconnect(). to disconnect the socket connection
// in: cid. The Connection ID (returned from wifi_tcp_connect())
// ret: 0-OK, else Failed
extern int wifi_tcp_disconnect(int cid);

// wifi_tcp_write(). to write a stream data to the specify socket
// in: cid: Connection ID (returned from wifi_tcp_connect())
// in: data: pointer to data buffer to be write
// in: datelen: len of the data
// ret: 0-OK. Else failed
extern int wifi_tcp_write(int cid, unsigned char* data, int datalen);

// wifi_tcp_read(). to read data stream in the socket
// Note: after read and processing on data completed. Programmer must clean the data by using wifi_tcp_flush();
// in/out: cidptr. Pointer to CID. For listening mode connection, the function will overwrite this CID to tell which CID is actually stream the data
// out: data. pointer of data pointer
// int: timeoutms: the waiting time out value in ms to wait for incoming data stream
// ret: the length of stream data. Negative is Error
extern int wifi_tcp_read(int* cidptr, unsigned char** data, int timeoutms);

// wifi_tcp_flush(). to flush the data in instream strorage 
extern int wifi_tcp_flush(int cid, int flushlen);

/****************************************************************************************************************
*	
* H2CORE - SAM FUNCTIONS (extended from sam.h)
*
*****************************************************************************************************************/

// sam_connect() - power up and connect to a SAM
// in:	slotid - SAM Slot ID (1, 2)
// ret:	0-Successful, Other-Error
extern unsigned char sam_connect(unsigned char slotid);

// sam_get_status() - obtain the ATR of a connected SAM
// in:	slotid - SAM Slot ID
// out:	atr - pointer to a buffer to store atr
// out: atrlen - len of atr
// ret:	0-Successful, Other-Error
extern unsigned char sam_get_status(unsigned char slotid, unsigned char** atr, int* atrlen);

// sam_transmit() - APDU exhange
// in:	slotid - SAM Slot ID
// in:	apdu - pointer to apdu buffer
// in:	apdulen - length of apdu
// out:	resp - pointer to store response from SAM
// out: resplen - lengtht of response
// ret:	0-Successful, Other-Error
extern unsigned char sam_transmit(unsigned char slotid, unsigned char* apdu, int apdulen, unsigned char* resp, int* resplen);

// sam_disconnect - power off and disconnect a SAM
// in:	slotid - SAM Slot ID
// ret:	0-Successful, Other-Error
extern unsigned char sam_disconnect(unsigned char slotid);


/****************************************************************************************************************
*	
* H2CORE - PCD FUNCTIONS (extended from pcd.h)
*
*****************************************************************************************************************/

// pcd_get_id() - to obtain the product infor stored in pcd chipset
// out: productinfo8 - 8 byte of product info
// ret: 0-successful, negative error
extern unsigned char pcd_get_id(unsigned char* productinfo8);//obtain pcd hardwar info

// pcd_rf_off() - turn OFF the PCD RF field
// ret: 0-successful, negative-error
extern unsigned char pcd_rf_off(void);

// pcd_rf_on() - turn ON the PCD RF field
// ret: 0-successful, negative-error
extern unsigned char pcd_rf_on(void);

// pcd_rf_off() - turn OFF the PCD RF field
// ret: 0-successful, negative-error
extern unsigned char pcd_rf_reset(void);

// pcd_is_rf_on() - to check whether the RF is ON
// ret: 1-RF is ON, 0-RF is OFF, negative=PCD error
extern int pcd_is_rf_on(void);

// pcd_reqa() - send ISO14443A request A
// out:	atqa - 2 bytes ATQA response from card
// ret: 0-successful, Other=error
extern unsigned char pcd_reqa(unsigned char* atqa);
extern unsigned char pcd_wupa(unsigned char* atqa);
extern unsigned char pcd_anticol(unsigned char level, unsigned char* uid);
extern unsigned char pcd_select(unsigned char level, unsigned char* uid, unsigned char* sak);
extern unsigned char pcd_hlta(void);
extern unsigned char pcd_rats(unsigned char parameter, unsigned char* ats, int* atslen);

//MIFARE FUNCTION
#define MI_KEYTYPE_A	0x0A
#define MI_KEYTYPE_B	0x0B
extern unsigned char pcd_mi_auth(unsigned char blockno, unsigned char keytype, unsigned char* key);
extern unsigned char pcd_mi_read(unsigned char blockno, unsigned char* Resp, int* RespLen);
extern unsigned char pcd_mi_write(unsigned char blockno, unsigned char* datain);
extern unsigned char pcd_mi_increment(unsigned char blockno, unsigned char* value);
extern unsigned char pcd_mi_decrement(unsigned char blockno, unsigned char* value);
extern unsigned char pcd_mi_retore(unsigned char blockno);
extern unsigned char pcd_mi_transfer(unsigned char blockno);
extern unsigned char pcd_mi_incrementtransfer(unsigned char blockno, unsigned char* value);
extern unsigned char pcd_mi_decrementtransfer(unsigned char blockno, unsigned char* value);
extern unsigned char pcd_mi_readvalue(unsigned char blockno, unsigned char* value, unsigned char* addrdata);
extern unsigned char pcd_mi_writevalue(unsigned char blockno, unsigned char* value, unsigned char addrdata);


// pcd_reqb() - send ISO14443B request B
// out: resp - pointer to buffer of ATQB response
// out: resplen - length of ATQB
// ret: 0-successful, negative-error
extern unsigned char pcd_reqb(unsigned char* resp, int* resplen);
extern unsigned char pcd_wupb(unsigned char* resp, int* resplen);

extern unsigned char pcd_attrib(unsigned char* atqb, int atqblen, unsigned char* resp, int* resplen);
/*
extern unsigned char pcd_settimeout(unsigned char fwi, unsigned char swtx);
extern unsigned char pcd_transa(unsigned char* apdu, int apdulen, unsigned char* resp, int* resplen);
extern unsigned char pcd_transb(unsigned char* apdu, int apdulen, unsigned char* resp, int* resplen);*/
extern unsigned char pcd_transa_to(unsigned char* apdu, int apdulen, unsigned char* resp, int* resplen, unsigned char fwi);
extern unsigned char pcd_transb_to(unsigned char* apdu, int apdulen, unsigned char* resp, int* resplen, unsigned char fwi);

// pcd_sc_list() - list the reader name
// out: readerlist - list of reader available, separated by ';' if more than one reader appear
// ret: 0-successful, other-error
extern unsigned char pcd_sc_list(char* readerlist);

// pcd_sc_connect() - connect to a contactless card that place on top
// ret: 0-No Card, 1-One Card, 2-More Card, 0xFF, Card Read Error
extern unsigned char pcd_sc_connect(void);

// pcd_sc_status() - obtain the ATS status from the connected card
// out: cardinfo - atr buffer
// out: cardinfolen - pointer to ats length
// ret: 0x00 OK, Other Error
extern unsigned char pcd_sc_get_status(unsigned char* cardinfo, int* cardinfolen);


// pcd_sc_transmit() - exchange apdu
// in: apdu - pointer to apdu
// in: apdulen - length of apdu
// out: resp - card response (include SW12 if any)
// out: resplen - response length
// ret: 0-Successful, Other-Failed(Error Code)
extern unsigned char pcd_sc_transmit(unsigned char* apdu, int apdulen, unsigned char* resp, int* resplen);

// pcd_sc_disconnect() - Card Removal
// ret: 0-Successful (Card Remove successfully), Other-Failed
extern unsigned char pcd_sc_disconnect(void);

#define PCD_SCCONTROL_SET_STOP_POLL		0x01 /*to set a button to stop card polling*/
extern unsigned char pcd_sc_control(unsigned char P1, unsigned char P2, unsigned char* data, int datalen);


/****************************************************************************************************************
*	
* H2CORE - DEBUG CHANNEL
*
*****************************************************************************************************************/
extern void h2core_set_debug(unsigned int hwid);
extern void h2core_clear_debug(unsigned int hwid);
extern unsigned char h2core_is_debug(unsigned int hwid);
extern void h2core_set_debug_console_output(unsigned int hwid);


/****************************************************************************************************************
*	
*H2CORE - SPEAKER FUNCTION
*
*****************************************************************************************************************/

// init_speaker() - initialize speaker
extern void init_speaker(void);

// set_speaker() - set speaker on/off
// in:	0 - disable speaker
//		1 - enable speaker
extern void set_speaker(int status);

// audio_playfile() - play WAV file
// mode: 0 - stereo
//		 1 - mono
extern char audio_playfile(char* filename ,unsigned char mode);


extern void debugonlcd(char* message, unsigned char* data, int datalen);

/****************************************************************************************************************
*	
*H2CORE - LED RING FUNCTION
*
*****************************************************************************************************************/

// ledring_init() - initialize led ring
int ledring_init(void);

// set_led_ring() - set status of led ring
// in:	0x01 - enable led ring
//		0x00 - disable led ring
void set_led_ring(unsigned char status);

// led_ring_on() - turn on led ring (all 8 leds)
// in:	0x01 - blue
//		0x02 - yellow
//		0x03 - green
//		0x04 - red
//		0x05 - white
extern void led_ring_on(unsigned char colour);


// led_ring_off() - turn off led ring (all 8 leds)
extern void led_ring_off(void);

#endif /* H2CORE_H_ */