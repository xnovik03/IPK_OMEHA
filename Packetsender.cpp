#include "Packetsender.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <netinet/ip.h>    // Pro IP header
#include <netinet/tcp.h>   // Pro TCP header (tcphdr and flags jako TH_SYN)
#include <netinet/ip_icmp.h>
#include <netdb.h>
#include <errno.h>
#include <linux/errqueue.h>

Packetsender::PortState Packetsender::sendTcpPacket(const std::string &ip, int port) {
    int sockfd;
    struct sockaddr_in dest;
    struct sockaddr_in6 dest6;
    // Zjistíme, zda jde o IPv6 adresu (obsahuje znak ':')
    bool isIPv6 = ip.find(":") != std::string::npos;

    // IPv6
    if (isIPv6) {
        // Vytvoření socketu pro IPv6
        sockfd = socket(AF_INET6, SOCK_STREAM, 0);
        if (sockfd == -1) {
            std::cerr << "Socket creation failed for IPv6" << std::endl;
            return FILTERED;
        }
        // Nastavení timeoutu pro send a receive
        struct timeval timeout;
        timeout.tv_sec = 2;
        timeout.tv_usec = 0;
        setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
        setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
        // Naplnění struktury sockaddr_in6
        dest6.sin6_family = AF_INET6;
        dest6.sin6_port = htons(port);
        inet_pton(AF_INET6, ip.c_str(), &dest6.sin6_addr);
        // Pokus o připojení
        if (connect(sockfd, (struct sockaddr *)&dest6, sizeof(dest6)) == 0) {
            close(sockfd);
            return OPEN;
        } else {
            // Zjištění důvodu selhání pro určení stavu portu
            PortState result;
            switch (errno) {
                case ECONNREFUSED: result = CLOSED; break;
                case ETIMEDOUT:
                case EHOSTUNREACH:
                case ENETUNREACH: result = FILTERED; break;
                default: result = FILTERED; break;
            }
            close(sockfd);
            return result;
        }
    }

    // IPv4
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        std::cerr << "Socket creation failed for IPv4" << std::endl;
        return FILTERED;
    }
    // Timeouty stejně jako u IPv6
    struct timeval timeout;
    timeout.tv_sec = 2;
    timeout.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    dest.sin_family = AF_INET;
    dest.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &dest.sin_addr);
    // Pokus o připojení
    if (connect(sockfd, (struct sockaddr *)&dest, sizeof(dest)) == 0) {
        close(sockfd);
        return OPEN;
    } else {
        // Zjištění důvodu selhání
        PortState result;
        switch (errno) {
            case ECONNREFUSED: result = CLOSED; break;
            case ETIMEDOUT:
            case EHOSTUNREACH:
            case ENETUNREACH: result = FILTERED; break;
            default: result = FILTERED; break;
        }
        close(sockfd);
        return result;
    }
}



// Funkce pro odeslání UDP paketu (podpora IPv4 a IPv6)
Packetsender::PortState Packetsender::sendUdpPacket(const std::string &ip, int port) {
    int sockfd;
    int result;
    char msg[] = "Test";
    char buffer[512];
    struct addrinfo hints, *res;
    PortState state = OPEN;  // Výchozí: pokud nedostaneme ICMP chybovou zprávu, port považujeme za open

    memset(&hints, 0, sizeof(hints));
    // Nastavíme hints.ai_family podle formátu IP adresy:
    if (ip.find(":") != std::string::npos) {
        hints.ai_family = AF_INET6;  // IPv6
    } else {
        hints.ai_family = AF_INET;   // IPv4
    }
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;

    result = getaddrinfo(ip.c_str(), std::to_string(port).c_str(), &hints, &res);
    if (result != 0) {
        std::cerr << "getaddrinfo: " << gai_strerror(result) << std::endl;
        return CLOSED;
    }

    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd < 0) {
        std::cerr << "Failed to create UDP socket" << std::endl;
        freeaddrinfo(res);
        return CLOSED;
    }

    // Nastavení socketové možnosti pro přijímání ICMP chyb:
    int opt = 1;
    if (res->ai_family == AF_INET6) {
        if (setsockopt(sockfd, IPPROTO_IPV6, IPV6_RECVERR, &opt, sizeof(opt)) < 0) {
            std::cerr << "setsockopt(IPV6_RECVERR) failed" << std::endl;
            close(sockfd);
            freeaddrinfo(res);
            return CLOSED;
        }
    } else {
        if (setsockopt(sockfd, IPPROTO_IP, IP_RECVERR, &opt, sizeof(opt)) < 0) {
            std::cerr << "setsockopt(IP_RECVERR) failed" << std::endl;
            close(sockfd);
            freeaddrinfo(res);
            return CLOSED;
        }
    }

    // Nastavení timeoutu pro příjem (2 sekundy)
    struct timeval tv;
    tv.tv_sec = 2;
    tv.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    // Odeslání UDP paketu
    result = sendto(sockfd, msg, sizeof(msg), 0, res->ai_addr, res->ai_addrlen);
    if (result < 0) {
        std::cerr << "Error sending UDP packet to " << ip << ":" << port 
                  << " (" << strerror(errno) << ")" << std::endl;
        close(sockfd);
        freeaddrinfo(res);
        return CLOSED;
    }

    // Načtení odpovědi z chybové fronty (MSG_ERRQUEUE)
    struct msghdr msg_hdr;
    struct iovec iov;
    char control[512];
    memset(&msg_hdr, 0, sizeof(msg_hdr));
    memset(control, 0, sizeof(control));
    iov.iov_base = buffer;
    iov.iov_len = sizeof(buffer);
    msg_hdr.msg_iov = &iov;
    msg_hdr.msg_iovlen = 1;
    msg_hdr.msg_control = control;
    msg_hdr.msg_controllen = sizeof(control);

    result = recvmsg(sockfd, &msg_hdr, MSG_ERRQUEUE);
    if (result < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            // Nebyla obdržena žádná chybová zpráva – port považujeme za open
            state = OPEN;
        } else {
            std::cerr << "recvmsg(MSG_ERRQUEUE) error: " << strerror(errno) << std::endl;
            state = CLOSED;
        }
    } else {
        // Projdeme řídící zprávy a zjistíme, zda byla ICMP odpověď "port unreachable"
        struct cmsghdr *cmsg;
        bool icmp_received = false;
        for (cmsg = CMSG_FIRSTHDR(&msg_hdr); cmsg != nullptr; cmsg = CMSG_NXTHDR(&msg_hdr, cmsg)) {
            if (res->ai_family == AF_INET6) {
                if (cmsg->cmsg_level == IPPROTO_IPV6 && cmsg->cmsg_type == IPV6_RECVERR) {
                    struct sock_extended_err *sock_err = (struct sock_extended_err *)CMSG_DATA(cmsg);
                    if (sock_err && sock_err->ee_type == ICMP_UNREACH && sock_err->ee_code == ICMP_UNREACH_PORT) {
                        icmp_received = true;
                        break;
                    }
                }
            } else {
                if (cmsg->cmsg_level == IPPROTO_IP && cmsg->cmsg_type == IP_RECVERR) {
                    struct sock_extended_err *sock_err = (struct sock_extended_err *)CMSG_DATA(cmsg);
                    if (sock_err && sock_err->ee_type == ICMP_UNREACH && sock_err->ee_code == ICMP_UNREACH_PORT) {
                        icmp_received = true;
                        break;
                    }
                }
            }
        }
        state = icmp_received ? CLOSED : OPEN;
    }

    close(sockfd);
    freeaddrinfo(res);
    return state;
}

   