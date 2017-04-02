/*
 * tcp.h
 *
 * Created: 8/6/2016 5:57:33 PM
 *  Author: KM
 */ 

#ifndef TCP_H_
#define TCP_H_



//return >= 0 indicate Connection ID, OK
//return negative indicate error
int tcp_connect(char* ip, char* port);

int tcp_disconnect(int cid);

//TCP Write
//write data into a connected TCP
//cid=connection id return from tcp_connect
//data = data to send
//datalen = length of data to send
int tcp_write(int cid, unsigned char* data, int datalen);

//TCP Read
//cid that return from tcp_connect upon successful
//data = pointer of pointer to buffer address where the responded data is stored.
//tcp_flush is required in order to remove the data from buffer
//timeout is ms
//return the length of read
int tcp_read(int* cidptr, unsigned char** data, int timeoutms);

int tcp_flush(int ConnectionId, int flushlen);


//transient read = tcp_read, tcp_flush, no data will be keep in buffer
//return positive indicate length of read, successful
//return negative indicate failed, error code
int tcp_tsread(int cid, unsigned char* resp, int timeOutMs);

//transient transmit = tcp_write, tcp_read, tcp_flush, no data will be keep in buffer
//return positive indicate length of read, successful
//return negative indicate failed, error code
int tcp_tstransmit(int cid, unsigned char* data, int datalen, unsigned char* resp, int timeOutMs);


#endif /* TCP_H_ */