/*
 * fposix.h
 *
 * Created: 2012-04-04 6:37:01 PM
 *  Author: Justin
 */ 



#ifndef FPOSIX_H_
#define FPOSIX_H_

#define O_CREATE		0x0001
#define O_READ			0x0002
#define O_WRITE			0x0004
#define O_APPEND		0x0008

#define FP_ER_OK				(0)
#define FP_ER_INVALID_HANLDE	(-1)
#define FP_ER_ACCESS_NF_ERR			(-10)
#define FP_ER_FILE_NOT_FOUND			(-11)
#define FP_ER_FILE_FULL				(-12)
#define FP_ER_FILE_ALREADY_EXIST		(-13)
#define FP_ER_INVALID_HANDLE			(-14)
#define FP_ER_OVER_ACCESS				(-15)
#define FP_ER_CRITICAL					(-16)
#define FP_ER_NOT_ALLOWED				(-17)
#define FP_ER_VOLUME_EMPTY				(-18)

/*
Get file name
- *filename(out) - the buffer to store the next file name
- *filsize(out) - int buffer ptr to store file size
- new(in) - relist the file from start again
- RETURN - the size of the filename
*/
extern int pflistfile(char* filename, int* filesize, unsigned char new);

/*
Open file
- *filename(in) - the pointer to file name
- access(in) the access request to the file (O_CREATE, O_READ, O_WRITE, O_APPEND) 
- RETURN - Positive-the File Handle ID, Negative-Error
*/
extern int pfopen(char* filename, unsigned short access);

/*
Close file - close in opened file by providing the FID
- *pid(in) - the posix file handle ID
- RETURN - 0-OK, Negative-Error
*/
extern int pfclose(int pid);



extern int pfgetsize(int pid);

/*
Write data into file (Append Mode)
- pid - posixfile handle from successful open() function 
- data - pointer of data to be writen into file
- datalen - length of data to be written
- RETURN - Total size of file after succesful writting. Negative indicate error 
*/
extern int pfwrite(int pid, unsigned char* data, int datalen);

/*
Read data from a file
- pid - posixfile handle from successful open() function 
- readoffset - the start index of data to read from a file. if readoffset exceed the file size, error with returned
- readlen - the size of data to read. if readlen = 0, then read full file, which is not recommended for large file
- *data - a pointer to RAM buffer to store the read data
- RETURN - size of read, negative indicate error
*/
extern int pfread(int pid, int readoffset, int readlen, unsigned char* data);

/*
Set a single character (byte) in a file
- pid - posixfile handle from successful open() function 
- Offset - offset of data to be set
- data - single byte data to be written
- RETURN - Total size of file after successful writting. Negative indicate error 
*/
int pfsetchar(int pid, int offset, unsigned char charater);

/*
Get string from stream
Reads characters from stream and stores them as a C string into str until (num-1) characters have been read or either a newline or the End-of-File is reached, whichever comes first.
A newline character makes fgets stop reading, but it is considered a valid character and therefore it is included in the string copied to str.
A null character is automatically appended in str after the characters read to signal the end of the C string.

pid - posixfile handle from successful open() function 
str - Pointer to an array of chars where the string read is stored.
num - Maximum number of characters to be read (including the final null-character). Usually, the length of the array passed as str is used.

Return Value
	On success, the function returns the same str parameter.
	If the End-of-File is encountered and no characters have been read, the contents of str remain unchanged and a null pointer is returned.
	If an error occurs, a null pointer is returned.
*/
extern char* pfgets(int pid, char* str, int num);

extern int pfdelete(char* filename);

/*
Format the entier volume
*/
extern int pfformat(void);

#endif /* NF_LINK_H_ */