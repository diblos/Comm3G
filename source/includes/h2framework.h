/*
 * h2framework.h
 *
 * Created: 2012-04-23 11:52:14 AM
 *  Author: Justin
 */ 




/****************************************************************************************************************	
*H2FRAMEWORK - VERSIONING
Version		Date		Description
0.1.0.0		20120417	Beta Release
0.2.0.0		20120427	Added main activity in framework - main activity will display framework and core info
0.3.0.0		20120427	Added fs_get_file size in scpi (0x05 cmd)
0.3.1.0		20120428	Fixes: Solve List View Flicking problem
0.3.2.0		20120503	Changes: Debug message size enlarged from 1000 to 2000 char len
0.4.0.0		20120503	Added: List view control on adjusting option value
0.5.0.0		20120525	Added: Wifi Setting
0.5.1.0		20120525	Small Update: Updated SCPI to support system get info commands
0.6.0.0		20120525	Added: SCPI supported over WIFI TCP Socket
0.6.1.0		20120529	Small Update: Added msgbox_showdialog(...) - blocking method msgbox
0.6.2.0		20120530	FineTune: WIFI connect and disconnect sequence handling faster (This update moved to h2core)
0.7.0.0		20120531	Added: Wifi, USB, KeyInputType Icon on status bar. Key Input now support alphabetic (Upper, Lower), Numeric, and symbolic
0.8.0.0		20120605	Added: Dispaly Text Box in multiple line. MsgBox ready with multiple line
0.9.0.0		20120620	Added: STATIC IP/DHCP Configuration on menu
						Added: use INI to store WIFI configuration and keys
0.A.0.0	20120621	Added: SCPI to support Mifare Function
						Added: SCPI to support ISP donwload
0.A.1.0		20120626	BugFixed: Disable Activity Clock now ok
0.B.0.0		20120627	Added: GUI List View ready with ON_OFF button, and CHECKBOX
0.B.1.0		20120628	Added: GUI List View support Volume Bar, Scroller Bar
0.B.2.0		20120629	BugFixed: Touch Control. Now enable reset on new activity
						BugFixed: Touch Contorl Long Press Event
						BugFixed: Start Activity now accept input data	
0.C.0.0		20120704	Added: Framework Screen Saver Function
0.C.1.0		20120711	BugFixed: Multiline textbox missing 1char (Front char) problem
0.C.2.0		20120718	Bugfixed: Alphabet button. Wrong PRQS, corrected to PQRS
						Changes: Input textbox size change to 36 with 24 sized font MsSerif
0.D.0.0		20120723	Added: Battery Metering Display in real		
0.E.0.0		20120823	Added: Hardware callback function from h2core to h2frameowrk( to notify something happen)
						Added: Auto power off when battery voltage fall under 35500
						Added: Power management contorl. sys sleep, and sys wake up		
0.E.1.0		20120903	Added: SCPI to support PCD_SC_GET_STATUS
0.F.0.0		20120911	Added: Touch control to support button button (Sleep, & Turn Off)

0.F.0.1     20120923	Changes: Transa & Transb in SCPI command change drom 0x40 to 0x41 for type A, and 0x50 to 0x51 for type B
						added listview animatedelay
						BugFixed: Wifi Advance Setting on DHCP
0.10.0.0	20121002	Changes: Battery meter update accoring to hardware version WFRD_V32 & WFRD_TOUCH_V32

0.11.0.0	20121017	utils files removed from this project, and created a new project call lib_util.a
						h2framworks and other project now required to include this header and lib file		
						added: PCD Setting Activity
0.11.1.0	20121017	Fixes: Fix touch button slow respnose problem. by reduce get_input_key during touch_task();
0.12.0.0	20121020	Added reset PCD info
						Added WIFI ON/OFF (PCD Board version 2012-10-20 onward)
0.12.1.0	20121020	added WIFI AT CMD at SCPI
0.13.0.0	20121023	Added Magswitch Function
0.13.1.0	20121023	Added keypulse to send shutdown signal to touch pad
0.14.0.0	20121030	Added SCPI for LED Blinking
						Added power Management contorl menu to configure magswitch, screen saver, and standby time
0.15.0.0 	20121120	changes on scpi draw string
0.15.1.0	20121126	BugFixed: SCPI Mifare Read Value. Returning value is not correct (error with h2core)
0.16.0.0	20121206	Added: SCPI functions to exit_to_main_sector, and run_app_in_ram
0.17.0.0	20121207	BugFixed: Input Message Box. Fixed to max 32 character input
						Added: Input Message box for decimal and Amount
0.17.1.0	20121211	BugFixed: Msgbox get input for AMT, didnt return the updated value (Solved)
0.17.2.0	20121217	Enhanced: MSGBOX Input with STR format will start with alphabetic input
0.17.3.0	20121219	Enhanced: MSGBOX Input added with Number Plate input
0.18.0.0	20121226	Added: SCPI to call FileSystem SetCharacter function
						Enhanced: WIFI auto connect during startup if WIFI only ON after hammerhead turned ON
						Enhanced: Battery low notification service (during in sleep mode)
0.18.1.0	20121227	Bugfixed: Msgbox input for car plate (auto detection and 3rd digit alphabet problem)
0.18.2.0	20121228	Bugfixed: WIFI establishing - at border range when wifi connected and IP not assigned. WIFI connection considered failed, and should retry ATA
0.18.3.0	20121231	Added: Msgbox get input with input box only
0.19.0.0	20130112	Added: SCPI to support RS232 comm
0.19.1.0	20130112	Enhanced: Some drawing debug message not cleaned (Now Cleaned)
0.19.2.0	20130305	Added: Get Last SCPI instruction API (To be used by Ocean to terminate startup)
0.1A.0.0	20130321	Added: Commands: doutoff, doutusb, doutserial to set console output to NULL, USB and SERIAL
0.1B.0.0	20130412	Added: SCPI for SC Control, Set Debug Console
						Changes: Long Touch. keyPress will be appended immediately
0.1B.1.0	20130508	BugFixed: Debug Print (now no need to set debug port in framework, all control by core printf)
0.1C.0.0	20130516	Added: Restart menu in Power Menu
0.1D.0.0	20130719	Added: Status Bar Scheme ID 0x00 for default, and 0x01 for desktop white
0.1D.1.0	20130729	Bugfixed: WIFI button ON/OFF now check from WIFI_STATUS
1.0.0.0		20130807	Added: Screen Saver on standard file with header "ssaver_" - cimage
						Enhanced: Screen Saver disabled in flash app (Ocean)
1.1.0.0		20130813	Bugfixed: Screen saver exit when calling set_timer(...) with 0 should exit ssaver		
1.2.0.0		20130816	Bugfixed: Screen saver hang after the 5th display (last one) counter pointer problem
1.2.1.0		20130816	Bugfixed: Screen saver enable need to do sys_touch()		
1.2.2.0		20130823	Enhanced: WIFI SSID listing up to 32 list		
1.3.0.0		20130830	Enhanced: Screen Saver Slot increased to 10 slots (from 5)
1.3.1.0		20130903	Bugfixed: Activity Silent for MSGBOX for next new act start
1.3.2.0		20131108	Enhanced: Max String size for Listview item label & value changed from 32 to 48
									ListView display support DESC display with Font MsSerif 18
						** NOTE: REQUIRED TO UDPATE GUI.H **
1.4.0.0		20131112	Added: ScreenSaver added with set intervalms function
1.5.0.0		20131113	Added: Activity Exit Util - exit existing activities (level by level) until the id matched

*****************************************************************************************************************/
#ifndef H2FRAMEWORK_H_
#define H2FRAMEWORK_H_

#define H2FRAMEWORK_VERSION		0x01050000
#define H2FRAMEWORK_RDATE		0x20131113

//#define H2FRAMEWORK_FOR_OCEAN

// h2framework_init() - register application to the framework. every app that call on top of framework must register to framework via this function
// in: applicationName - name of application (string), max 24 chars
// in: version - application version, 4 bytes in unsigned int
extern void h2framework_init(char* applicationName, unsigned int version);

extern void h2framework_task(void);

// h2framework_get_version - obtain framework version. MMMMNNNN
// ret: version in unsigned int which is 4 bytes, most significant 2 bytes indicate major version, least 2 bytes indicate minor version
extern unsigned int h2framework_get_version(void);

//h2framework_show_abot() - this function will prompt up a listview and display the framework + core inforamtion
extern void h2framework_show_about(void);
//h2framework_show_setting() - this function will prompt up a listview and display the device setting
extern void h2framework_show_setting(void);

extern void h2framework_lcd_toggle(unsigned char upsidedown, unsigned char refreshactivity);

//
// Power Mgmt & Screen Saver F(x)
//
extern int h2framework_ssaver_add(unsigned short* colorImage);
extern int h2framework_ssaver_set_timer(unsigned long idletimems);
extern void h2framework_power_set_sleep(unsigned long periodms);
extern int h2framework_ssaver_set_interval(unsigned long intervalms);

#endif /* H2FRAMEWORK_H_ */