//by Vasily Yuryev

#ifndef UDP_LOWLEVEL_H
#define UDP_LOWLEVEL_H

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

/* Linux / MacOS POSIX timer headers */
#include <sys/time.h>
#include <time.h>
#include <arpa/inet.h>
#include <stdbool.h> /* required for the definition of bool in C99 */


/* This assumes you have the mavlink headers on your include path
 or in the same folder as this source file */
#include <string>
#include <vector>
#include <algorithm>
#include "mavlink.h"
#include "lowlevel.h"

using namespace std;

#define BUFFER_LENGTH 2041 // 2041

namespace mavlink_indoor_sdk
{

class UDP_Protocol : public LowLevel
{
public:
    UDP_Protocol();
    UDP_Protocol(string url);

    void start();
    void start(string url);

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
    struct sockaddr_in addr;
    struct sockaddr_in src_addr;
    struct Remote
    {
        std::string ip{};
        int port_number{0};

        bool operator==(const UDP_Protocol::Remote &other)
        {
            return ip == other.ip && port_number == other.port_number;
        }

        uint8_t system_id{0};
    };
    std::vector<Remote> remotes{};
    // const socklen_t addrlen = sizeof(sockaddr);

    // ssize_t recsize;
    // int bytes_sent;
    // socklen_t fromlen;
    // uint16_t len;
    // unsigned int temp = 0;
};
}; // namespace mavlink_indoor_sdk

#endif