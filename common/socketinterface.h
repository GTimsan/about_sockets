#include "my_socket_lib.h"


class SocketInterface {
public:
    SocketInterface();
    
    void default_settings_TCP();
    void settings(int family, int type, int protocol, 
                    std::string ip_addr, uint16_t port);
    void setAddressIp4(std::string ip_addr);
    void setPort(uint16_t port);
    void setFamily(int family);
    void setSocket(int family, int type, int protocol);
    sockaddr* getSockaddr();
    int getSockfd();
    virtual void start() =0;

protected:
    int sockfd;
    sockaddr_in servaddr;
};