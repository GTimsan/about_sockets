#include "client.h"



void Client::start() {
}

int Client::connect() {
		int n;
        n = Connect(sockfd, (sockaddr*)&servaddr, sizeof(servaddr));
		return n;
}

int Client::send_message(void* arr, u_int16_t port, std::string ip_address) {
	int poll_result;
	while (true) {
		poll_result = Poll(fds, 1, 10);
		if (!connect_to_serv) {
			default_settings_TCP();
			setPort(port);
			setAddressIp4(ip_address);
			fd_for_client = getSockfd();
			fds[0].fd = fd_for_client;
			fds[0].events = POLLHUP;
			if (connect() == 0) {
				connect_to_serv = true;
				Writen(fd_for_client, arr, 2);
				return 0;
			}
		} else {
			Writen(fd_for_client, arr, 2);
			poll_result = Poll(fds, 1, 10);
			if (poll_result == 0) {
				return 0;
			}
		}
		if (poll_result == 0) {
		} else if (!connect_to_serv) {
			return -1;
		} else if (poll_result > 0){
			char buf[MAXLINE] = {0};
			if (Readline(fd_for_client, buf, MAXLINE) == 0) {
						connect_to_serv = false;
						fds[0].revents = 0;
			}
			continue;
		}
	}
}

