#include "socketinterface.h"


SocketInterface::SocketInterface(): sockfd(0) {
    bzero(&servaddr, sizeof(servaddr));
}

void SocketInterface::default_settings_TCP() {
    settings(AF_INET, SOCK_STREAM, 0, "127.0.0.1", SERV_PORT);
}

void SocketInterface::settings(int family, int type, int protocol, std::string ip_addr, uint16_t port) {
    setSocket(family, type, protocol);
    setAddressIp4(ip_addr);
    setPort(port);
    setFamily(family);
}

void SocketInterface::setSocket(int family, int type, int protocol) {
    sockfd = Socket(family, type, protocol);
}

void SocketInterface::setAddressIp4(std::string ip_addr) {
    Inet_pton(AF_INET, ip_addr.c_str(), &servaddr.sin_addr);
}

void SocketInterface::setPort(uint16_t port) {
    servaddr.sin_port = htons(port);
}

void SocketInterface::setFamily(int family) {
    servaddr.sin_family = family;
}

sockaddr* SocketInterface::getSockaddr() {
    return (sockaddr*) &servaddr;
}

int SocketInterface::getSockfd() {
    return sockfd;
}