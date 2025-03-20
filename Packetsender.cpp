#include "Packetsender.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <netinet/ip.h>
#include <netinet/tcp.h>

// Funkce pro odeslání TCP SYN paketu
bool Packetsender::sendTcpSynPacket(const std::string &ip, int port) {
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if (sockfd < 0) {
        std::cerr << "Socket creation failed for TCP SYN" << std::endl;
        return false;
    }

    struct sockaddr_in dest;
    struct iphdr iph;
    struct tcphdr tcph;
    char packet[4096];

    // Nastavení IP hlavičky
    memset(&packet, 0, sizeof(packet));
    dest.sin_family = AF_INET;
    dest.sin_port = htons(port);
    dest.sin_addr.s_addr = inet_addr(ip.c_str());

    // Nastavení IP hlavičky
    iph.ip_hl = 5;
    iph.ip_v = 4;
    iph.ip_tos = 0;
    iph.ip_len = sizeof(struct iphdr) + sizeof(struct tcphdr);
    iph.ip_id = htonl(54321);
    iph.ip_off = 0;
    iph.ip_ttl = 255;
    iph.ip_p = IPPROTO_TCP;
    iph.ip_src.s_addr = inet_addr("192.168.1.1");  // Místní IP adresa
    iph.ip_dst.s_addr = dest.sin_addr.s_addr;
    iph.ip_sum = 0;  // Automatický výpočet checksumu

    // Nastavení TCP hlavičky
    tcph.th_sport = htons(12345);  // Náhodný port
    tcph.th_dport = htons(port);
    tcph.th_seq = 0;
    tcph.th_ack = 0;
    tcph.th_off = 5;
    tcph.th_flags = TH_SYN;  // SYN flag pro zahájení handshaku
    tcph.th_win = htons(5840); /* Maximum window size */
    tcph.th_sum = 0;  // Necháme to na knihovně pro checksum

    // Spojení IP a TCP hlaviček do paketu
    memcpy(packet, &iph, sizeof(struct iphdr));
    memcpy(packet + sizeof(struct iphdr), &tcph, sizeof(struct tcphdr));

    // Výpočet checksumu
    iph.ip_sum = checksum((unsigned short *)&iph, sizeof(struct iphdr));
    tcph.th_sum = checksum((unsigned short *)&tcph, sizeof(struct tcphdr));

    // Odeslání SYN paketu
    if (sendto(sockfd, packet, iph.ip_len, 0, (struct sockaddr *)&dest, sizeof(dest)) < 0) {
        std::cerr << "Error sending SYN packet to " << ip << ":" << port << std::endl;
        close(sockfd);
        return false;
    }

    std::cout << "SYN packet sent to " << ip << ":" << port << std::endl;

    close(sockfd);
    return true;
}

// Výpočet checksumu pro IP a TCP hlavičky
unsigned short Packetsender::checksum(unsigned short *buf, int len) {
    unsigned long sum = 0;
    while (len > 1) {
        sum += *buf++;
        len -= 2;
    }
    if (len == 1) {
        sum += *(unsigned char *)buf;
    }
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    return ~sum;
}
