#include "udp.h"
#include <iostream>
namespace mavlink_indoor_sdk
{
UDP_Protocol::UDP_Protocol(string url)
{
    // string target_ip =
    target_ip = url.substr(url.find_last_of('/') + 1, url.find_last_of(':') - (url.find_last_of('/') + 1));
    port = stoi(url.substr(url.find_last_of(':') + 1, url.length() - (url.find_last_of(':') + 1)));
    std::cout << "port" << port << " ip " << target_ip << std::endl;
    // socklen_t fromlen = sizeof(gcAddr);
    // memset(&locAddr, 0, sizeof(locAddr));
    // locAddr.sin_family = AF_INET;
    // locAddr.sin_addr.s_addr = INADDR_ANY;
    // locAddr.sin_port = htons(14551);

    // memset(&gcAddr, 0, sizeof(gcAddr));
    // gcAddr.sin_family = AF_INET;
    // gcAddr.sin_addr.s_addr = inet_addr(target_ip.c_str());
    // gcAddr.sin_port = htons(14550);
    // status = true;
}
int UDP_Protocol::write_message(mavlink_message_t &message)
{
    // len = mavlink_msg_to_send_buffer(buf_t, &message);
    // std::cout << "sending\n";
    // int bytes_sent = sendto(sock, buf_t, len, 0, (struct sockaddr *)&gcAddr, sizeof(struct sockaddr_in));
    // return bytes_sent;
    uint8_t data[MAVLINK_MAX_PACKET_LEN];

    uint16_t len = mavlink_msg_to_send_buffer(data, &message);
    // int r = sendto(sock, data, len, 0, (struct sockaddr *)&sockaddr, sizeof(sockaddr));
    int r = sendto(sock, data, len, 0, (struct sockaddr*)&sockaddr, sizeof(struct sockaddr));
    if (r < 0) {
        fprintf(stderr, "Could not send to %d: r=%d (%m)\n", sock, r);
    }
    else {
        // std::cout << "ok\n";
    }

    return r;
}
int UDP_Protocol::read_message(mavlink_message_t &message)
{
    // memset(buf_r, 0, BUFFER_LENGTH);
    mavlink_message_t msg;
    // std::cout << "reciving\n";
    uint8_t buf[1024];
    ssize_t n = read(sock, buf, sizeof(buf));
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
void UDP_Protocol::start()
{
    sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == -1) {
        fprintf(stderr, "Could not create socket (%m)\n");
        status = 0;
    }

    // bzero(&sockaddr, addrlen);
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = inet_addr(target_ip.c_str());
    sockaddr.sin_port = htons(port);

    if (bind(sock, (struct sockaddr *)&sockaddr, sizeof(struct sockaddr)) == -1) {
        fprintf(stderr, "Could not bind to %s:%d (%m)", target_ip.c_str(), port);
        // free(target_ip);
        // return 1;
        status = 0;
    }
    else {
        status = 1;
    }

	if (fcntl(sock, F_SETFL, O_NONBLOCK | O_ASYNC) < 0)
    {
		fprintf(stderr, "error setting nonblocking: %s\n", strerror(errno));
		// close(sock);
		// exit(EXIT_FAILURE);

        status = 0;
    }
}
void UDP_Protocol::stop()
{
}
}; // namespace mavlink_indoor_sdk