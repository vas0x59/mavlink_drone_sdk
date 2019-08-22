#include "udp.h"
#include <iostream>
namespace mavlink_indoor_sdk
{

UDP_Protocol::UDP_Protocol(string url)
{
    // string target_ip =
    target_ip = url.substr(url.find_last_of('/') + 1, url.find_last_of(':') - (url.find_last_of('/') + 1));
    port = stoi(url.substr(url.find_last_of(':') + 1, url.length() - (url.find_last_of(':') + 1)));
    // std::cout << "port" << port << " ip " << target_ip << std::endl;
}

UDP_Protocol::UDP_Protocol()
{
}

int UDP_Protocol::write_message(mavlink_message_t &message)
{
    // len = mavlink_msg_to_send_buffer(buf_t, &message);
    // std::cout << "sending\n";
    // int bytes_sent = sendto(sock, buf_t, len, 0, (struct sockaddr *)&gcAddr, sizeof(struct sockaddr_in));
    // return bytes_sent;
    int r = -1;
    for (auto &remote : remotes)
    {
        struct sockaddr_in dest_addr {};
        dest_addr.sin_family = AF_INET;

        inet_pton(AF_INET, remote.ip.c_str(), &dest_addr.sin_addr.s_addr);
        dest_addr.sin_port = htons(remote.port_number);
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];

        uint16_t len = mavlink_msg_to_send_buffer(buffer, &message);
        // int r = sendto(sock, data, len, 0, (struct sockaddr *)&sockaddr, sizeof(sockaddr));
        r = sendto(
            sock,
            reinterpret_cast<char*>(buffer),
            len,
            0,
            reinterpret_cast<const sockaddr*>(&dest_addr),
            sizeof(dest_addr));
        if (r < 0)
        {
            fprintf(stderr, "Could not send to %d: r=%d (%m)\n", sock, r);
        }
        else
        {
            // std::cout << "ok\n";
        }
    }

    return r;
}
int UDP_Protocol::read_message(mavlink_message_t &message)
{
    // memset(buf_r, 0, BUFFER_LENGTH);
    mavlink_message_t msg;
    // std::cout << "reciving\n";
    uint8_t buf[1024];
    // ssize_t n = read(sock, buf, sizeof(buf));
    socklen_t src_addr_len = sizeof(src_addr);
    ssize_t n = recvfrom(
        sock,
        buf,
        sizeof(buf),
        0,
        reinterpret_cast<struct sockaddr *>(&src_addr),
        &src_addr_len);
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
    if (received)
    {
        Remote new_remote;
        new_remote.ip = inet_ntoa(src_addr.sin_addr);
        new_remote.port_number = ntohs(src_addr.sin_port);

        auto existing_remote =
            std::find_if(remotes.begin(), remotes.end(), [&new_remote](Remote &remote) {
                return (
                    remote.ip == new_remote.ip &&
                    remote.port_number == new_remote.port_number);
            });

        if (existing_remote == remotes.end())
        {
            // std::cout << "New system on: " << new_remote.ip << ":"
            //           << new_remote.port_number << "\n";
            LogInfo("UDP Connection", "New system on: " + new_remote.ip + ":" + to_string(new_remote.port_number));
            remotes.push_back(new_remote);
        }
    }
    return received;
}
void UDP_Protocol::start()
{
    sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == -1)
    {
        // fprintf(stderr, "Could not create socket (%m)\n");
        LogError("UDP Connection", "Could not create socke (" + string(strerror(errno)) + ")");
        status = 0;
    }

    // bzero(&sockaddr, addrlen);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(target_ip.c_str());
    addr.sin_port = htons(port);
    LogInfo("UDP Connection", "target_ip: " + target_ip + " port: " + to_string(port));
    if (bind(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr)) == -1)
    {
        // fprintf(stderr, "Could not bind to %s:%d (%m)", target_ip.c_str(), port);
        LogError("UDP Connection", "Could not bind (" + string(strerror(errno)) + ")");
        // free(target_ip);
        // return 1;
        status = 0;
    }
    else
    {
        status = 1;
    }

    if (fcntl(sock, F_SETFL, O_NONBLOCK | O_ASYNC) < 0)
    {
        // fprintf(stderr, "error setting nonblocking: %s\n", strerror(errno));
        LogError("UDP Connection", "error setting nonblocking (" + string(strerror(errno)) + ")");
        // close(sock);
        // exit(EXIT_FAILURE);

        status = 0;
    }
}
void UDP_Protocol::start(string url)
{
    target_ip = url.substr(url.find_last_of('/') + 1, url.find_last_of(':') - (url.find_last_of('/') + 1));
    port = stoi(url.substr(url.find_last_of(':') + 1, url.length() - (url.find_last_of(':') + 1)));
    // std::cout << "port" << port << " ip " << target_ip << std::endl;
    start();
}
void UDP_Protocol::stop()
{
}

void UDP_Protocol::handle_quit(int sig){
    LogInfo("UDP Connection", "STOP");
    try {
		stop();
	}
	catch (int error) {
		LogError("UDP Connection", "Warning, could not stop ");
	}
}
}; // namespace mavlink_indoor_sdk