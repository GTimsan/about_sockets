#include "../program1/client.h"

class Server : public Client{
public:

    void start();
    void so_reuseaddr();
    void listen();
    void polling();

private:
    int add_new_client(pollfd* client);
    void prepare_list_of_clients();
    void if_connect_reset_by_client(std::string ip_string, u_int16_t client_port);
    void if_client_aborted_connection(std::string ip_string, u_int16_t client_port);
    void what_does_the_server_do(u_char* serv_buf, 
                                    std::string ip_string, u_int16_t client_port);
    void get_address_and_port(u_int16_t &client_port, std::string &ip_string);

private:
    int i, maxi, connfd, fd;
    int nready;
    ssize_t n;
    char buf[MAXLINE];
    socklen_t clilen;
    pollfd client[OPEN_MAX];
    sockaddr_in cliaddr;
};

uint16_t b_to_uint16(u_char* buf);
void check_result(const uint16_t &summ, std::string &result_string);
