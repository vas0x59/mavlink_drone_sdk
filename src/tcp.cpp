#include "tcp.h"
#include <iostream>
namespace mavlink_indoor_sdk
{
TCP_Protocol::TCP_Protocol(string url)
{
    // string target_ip =
    target_ip = url.substr(url.find_last_of('/') + 1, url.find_last_of(':') - (url.find_last_of('/') + 1));
    port = stoi(url.substr(url.find_last_of(':') + 1, url.length() - (url.find_last_of(':') + 1)));
    std::cout << "port" << port << " ip " << target_ip << std::endl;
}

TCP_Protocol::TCP_Protocol()
{
}
int TCP_Protocol::write_message(mavlink_message_t &message)
{
    uint8_t data[MAVLINK_MAX_PACKET_LEN];

    uint16_t len = mavlink_msg_to_send_buffer(data, &message);
    int r = sendto(sock, data, len, 0, (struct sockaddr *)&sockaddr, sizeof(sockaddr));
    if (r < 0)
    {
        fprintf(stderr, "Could not send to %d: r=%d (%m)\n", sock, r);
    }
    else
    {
        // std::cout << "ok\n";
    }

    return r;
}
int TCP_Protocol::read_message(mavlink_message_t &message)
{
    // memset(buf_r, 0, BUFFER_LENGTH);
    mavlink_message_t msg;
    // std::cout << "reciving\n";
    // uint8_t buf[1024];
    uint8_t buf[MAVLINK_MAX_PACKET_LEN];
    socklen_t addrlen = sizeof(sockaddr);

    ssize_t n = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr *)&sockaddr, &addrlen);
    // std::cout << n;
    // std::cout << "reciving: " << recsize << "\n";
    uint8_t received = false;

    if (n > 0)
    {
        // mavlink_message_t msg;
        mavlink_status_t status;
        for (int i = 0; i < n; ++i)
        {
            // temp = buf_r[i];
            // printf("%02x ", (unsigned char)temp);
            if (mavlink_parse_char(MAVLINK_COMM_0, buf[i], &msg, &status))
            {
                // Packet received
                received = true;
                // printf("\nReceived packet: SYS: %d, COMP: %d, LEN: %d, MSG ID: %d\n", message.sysid, msg.compid, msg.len, msg.msgid);
            }
        }
        message = msg;
        // memset(buf_r, 0, BUFFER_LENGTH);
    }
    return received;
}
void TCP_Protocol::start()
{
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        fprintf(stderr, "Could not create socket (%m)\n");
        // return -1;
    }

    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = inet_addr(target_ip.c_str());
    sockaddr.sin_port = htons(port);

    printf("Connecting to TCP: %s:%i\n", target_ip.c_str(), port);

    if (connect(sock, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) != 0)
    {
        fprintf(stderr, "Could not connect to socket (%m)\n");
        // return -1;
    }

    if (fcntl(sock, F_SETFL, O_NONBLOCK | O_ASYNC) < 0)
    {
        fprintf(stderr, "Error setting socket sock as non-blocking");
    }
}

void TCP_Protocol::start(string url)
{
    target_ip = url.substr(url.find_last_of('/') + 1, url.find_last_of(':') - (url.find_last_of('/') + 1));
    port = stoi(url.substr(url.find_last_of(':') + 1, url.length() - (url.find_last_of(':') + 1)));
    std::cout << "port" << port << " ip " << target_ip << std::endl;
    start();
}

void TCP_Protocol::stop()
{
    close(sock);
}
}; // namespace mavlink_indoor_sdk