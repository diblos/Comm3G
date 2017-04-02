/*
 * ini.h
 *
 * Created: 2012-06-08 5:23:55 PM
 *  Author: Justin
 */ 


#ifndef INI_H_
#define INI_H_

/****************************************************************************************************************	
*INI PARSER - VERSIONING
Version		Date		Description
0.1.0.0		20120607	Beta Release
0.1.1.0		20120619	BugFixed: ini_save section and key placed wrong place. Solved
*****************************************************************************************************************/
#define INI_PARSER_VERSION		0x00010100
#define INI_PARSER_RDATE		0x20120607

extern int				ini_getboolean(char* inifilename, char* section, char* key, int Default);
extern char*			ini_getstring(char* inifilename, char* section, char* key, char* Default);
extern int				ini_getint(char* inifilename, char* section, char* key, int Default);
extern unsigned char	ini_setstring(char* inifilename, char* section, char* key, char* value);
extern unsigned char	ini_setint(char* inifilename, char* section, char* key, int value);
extern unsigned char	ini_setboolean(char* inifilename, char* section, char* key, int value);

void					ini_commit(void);
void					ini_free_resource(void);


#endif /* INI_H_ */