#include "server.h"


void Server::listen() {
    Bind(sockfd, (sockaddr*)&servaddr, sizeof(servaddr));
    Listen(sockfd, LISTENQ);
}

void Server::so_reuseaddr() {
    const int enable = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
        std::cerr << "setsockopt(SO_REUSEADDR) failed\n";
}

void Server::start() {
    listen();
    polling();
}

void Server::polling() {
    Client client_for_server;
    u_int16_t client_port;
    std::string ip_string;
    // -----------------------------------------------------------
    prepare_list_of_clients();

    for (;;) {
        nready = Poll(client, maxi + 1, INFTIM);
        if (add_new_client(client) == -1) continue;
        for (i = 1; i <= maxi; ++i) {
            if ((fd = client[i].fd) < 0) continue;
            if (client[i].revents & (POLLRDNORM | POLLERR)) {
                get_address_and_port(client_port, ip_string);
                u_char serv_buf[MAXLINE] = {0};
                if ((n = read(fd, serv_buf, MAXLINE)) < 0) {
                    if (errno == ECONNRESET) {
                        if_connect_reset_by_client(ip_string, client_port);
                    } else {
                        std::cerr << "SERVER > Read error\n";
                    }
                } else if (n == 0) {
                    if_client_aborted_connection(ip_string, client_port);
                } else {
                    what_does_the_server_do(serv_buf, ip_string, client_port);
                    // -----------------------------------------------------------
                    // client_for_server.send_message(serv_buf, 9812, "192.168.100.12");
                    // -----------------------------------------------------------
                }
                if (--nready <= 0) break;
            }   
        }
    }
}

int Server::add_new_client(pollfd* client) {
    if (client[0].revents & POLLRDNORM) {
        clilen = sizeof(cliaddr);
        connfd = Accept(sockfd, (sockaddr*)&cliaddr, &clilen);
        std::string ip_string = inet_ntoa(cliaddr.sin_addr);
        u_int16_t client_port = ntohs(cliaddr.sin_port);
        std::cout << "SERVER > NEW client connection: [" 
        << ip_string << "][" << client_port << "][Descriptor:" << connfd << "]\n";
        for (i = 1; i < OPEN_MAX; ++i) {
            if (client[i].fd < 0) {
                client[i].fd = connfd;
                break;
            }
        }
        if (i == OPEN_MAX) {
            std::cerr << "SERVER > Too many clients\n";
            exit(-1);
        }
        client[i].events = POLLRDNORM;
        if (i > maxi) maxi = i;
        if (--nready <= 0) return -1;
    }
    return 0;
}

void Server::prepare_list_of_clients() {
    client[0].fd = sockfd;
    client[0].events = POLLRDNORM;
    for (i = 1; i < OPEN_MAX; ++i) {
        client[i].fd = -1;
    }
    maxi = 0;

}
void Server::if_connect_reset_by_client(std::string ip_string, u_int16_t client_port) {
    std::cout << "SERVER > Connection reset by client: [" << ip_string 
    << "][" << client_port << "][Descriptor:" << fd << "]\n";
    Close(fd);
    client[i].fd = -1;
}

void Server::if_client_aborted_connection(std::string ip_string, u_int16_t client_port) {
    std::cout << "SERVER > Client [" << ip_string 
    << "][" << client_port << "][Descriptor:" << fd << "] aborted connection.\n";
    Close(fd);
    client[i].fd = -1;
}

void Server::what_does_the_server_do(u_char* serv_buf, 
                                std::string ip_string, u_int16_t client_port) {
    uint16_t summ = b_to_uint16(serv_buf);
    std::string result_after_check = "";
    check_result(summ, result_after_check);
    std::cout << "[" << ip_string 
    << "][" << client_port << "][Descriptor:" << fd << "] > Number " 
    << summ << " " << result_after_check << std::endl;

}

void Server::get_address_and_port(u_int16_t &client_port, std::string &ip_string) {
    sockaddr_in for_client_ip;
    socklen_t for_client_ip_len = sizeof(for_client_ip);
    getpeername(fd, (sockaddr*)&for_client_ip, &for_client_ip_len);
    client_port = ntohs(for_client_ip.sin_port);
    ip_string = inet_ntoa(for_client_ip.sin_addr);
}

void check_result(const uint16_t &summ, std::string &result_string) {
    if (summ / 10 >= 1) {
        if (summ % 32 == 0) {
            result_string += " mod 32 == 0 SUCCESS";
            return;
        }
        result_string += " more than two characters but not divisible by 32 FAILED";
    } else {
        result_string += " less than two characters FAILED";
    }
}

uint16_t b_to_uint16(u_char* buf) {
    uint16_t summ = 0;
    summ += buf[0];
    summ <<= 8;
    summ += buf[1];
    return summ;
}


int main() {

    Server server;
    server.default_settings_TCP();
    server.so_reuseaddr();
    // server.setAddressIp4("192.168.100.11");
    // server.setPort(9811);
    server.start();
    
    return 0;
} 

