/*
 * tlv.h
 *
 *  Created on: Jan 30, 2012
 *      Author: Justin
 */

#ifndef TLV_H_
#define TLV_H_

#define TLV_ER_TAG_NOT_FOUND	-1
#define TLV_ER_INALID_TAG		-2
#define TLV_ER_INALID_LEN		-3
#define TLV_ER_INALID_VALUE		-4

#define TLV_MAX_TAG_SIZE	4
#define TLV_MAX_NULL_LEN	32

extern int tlv_encode_tag(unsigned long tag, unsigned char* dest);
extern int tlv_encode_len(int len, unsigned char* dest);
extern int tlv_encode_value(unsigned char* value, int valuelen, unsigned char* dest);
extern int tlv_decode_len(unsigned char* lenptr, int* affectedlen);
extern unsigned long tlv_decode_tag(unsigned char* tagptr, int* affectedlen);
extern int tlv_get_next_tag(unsigned char* datain, int datainlen, unsigned long* tag, int* len_offset, int* len_size, int* value_offset, int* value_size);
extern int tlv_get_value(unsigned char* datain, int datainlen, unsigned long tag, unsigned char** value, int* endedoffset);
extern int tlv_get_tlv(unsigned char* datain, int datainlen, unsigned long tag, unsigned char** value, int* endedoffset);
extern int tlv_markoff_tag(unsigned char* datain, int datainlen, unsigned long tag);

#endif /* TLV_H_ */
