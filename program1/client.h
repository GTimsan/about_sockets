#include "../common/my_socket_lib.h"
#include "../common/socketinterface.h"



class Client : public SocketInterface {
public:
    void start();
    int connect();
    int send_message(void* arr, u_int16_t port = SERV_PORT, 
                    std::string ip_address = "127.0.0.1");
    
private:
    bool connect_to_serv = false;
    int fd_for_client;
    pollfd fds[1];
};

