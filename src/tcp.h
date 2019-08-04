//by Vasily Yuryev

#ifndef TCP_LOWLEVEL_H
#define TCP_LOWLEVEL_H


#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#if (defined __QNX__) | (defined __QNXNTO__)
/* QNX specific headers */
#include <unix.h>
#else
/* Linux / MacOS POSIX timer headers */
#include <sys/time.h>
#include <time.h>
#include <arpa/inet.h>
#include <stdbool.h> /* required for the definition of bool in C99 */
#endif

/* This assumes you have the mavlink headers on your include path
 or in the same folder as this source file */
#include <string>
#include "mavlink.h"
#include "lowlevel.h"

using namespace std;

#define BUFFER_LENGTH 2041 // 2041

namespace mavlink_indoor_sdk
{
class TCP_Protocol : public LowLevel
{
public:
    TCP_Protocol(string url);
    void start();
    void stop();

    int read_message(mavlink_message_t &message);
    int write_message(mavlink_message_t &message);
    int status;
    // void start();
    // void stop();

private:
    int port;
    string target_ip;
    int sock;
    // struct sockaddr_in gcAddr; 
	// struct sockaddr_in locAddr;
	// //struct sockaddr_in fromAddr;
	// uint8_t buf_r[BUFFER_LENGTH];
    // uint8_t buf_t[BUFFER_LENGTH];
    struct sockaddr_in sockaddr;
    const socklen_t addrlen = sizeof(sockaddr);

    // ssize_t recsize;
    // int bytes_sent;
    // socklen_t fromlen;
    // uint16_t len;
    // unsigned int temp = 0;

};
}; // namespace mavlink_indoor_sdk

#endif