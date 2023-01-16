#include "my_socket_lib.h"


int Poll(pollfd *fdarray, unsigned long nfds, int timeout) {
	int	n;
	if ( (n = poll(fdarray, nfds, timeout)) < 0)
		std::cerr << "Poll error\n";
	return n;
}

void Shutdown(int fd, int how) {
	if (shutdown(fd, how) < 0)
        std::cerr << "Shutdown error \n";
}

char* Fgets(char *ptr, int n, FILE *stream) {
	char	*rptr;
	if ( (rptr = fgets(ptr, n, stream)) == NULL && ferror(stream))
        std::cerr << "Fgets error \n";
	return rptr;
}

void Fputs(const char *ptr, FILE *stream) {
	if (fputs(ptr, stream) == EOF)
        std::cerr << "Fputs error \n";
}

ssize_t writen(int fd, const void *vptr, size_t n) {
	ssize_t nleft;
	ssize_t	nwritten;
	const char *ptr;
	ptr = (char*)vptr;
	nleft = n;
	while (nleft > 0) {
		if ( (nwritten = write(fd, ptr, nleft)) <= 0) {
			if (errno == EINTR)
				nwritten = 0;
			else
				return -1;
		}
		nleft -= nwritten;
		ptr   += nwritten;
	}
	return n;
}

ssize_t Writen(int fd, const void *ptr, size_t nbytes) {
	ssize_t n  = writen(fd, ptr, nbytes);
	if (n != (int)nbytes) {
        std::cerr << "Writen error\n";
	}
	return n;
}

void Close(int fd) {
	if (close(fd) == -1)
        std::cerr << "Close error\n";
}

int Inet_pton(int af, const char* address, void* buf) {
	int n = inet_pton(af, address, buf);
	if (n <= 0) {
		std::cerr << "Inet_pton error" << std::endl;
	}
	return n;
}

ssize_t readline(int fd, void *vptr, size_t maxlen) {
	ssize_t	n, rc;
	char c, *ptr;
	ptr = (char*)vptr;
	for (n = 1; n < (int)maxlen; n++) {
		if ( (rc = read(fd, &c, 1)) == 1) {
			*ptr++ = c;
			if (c == '\n')
				break;	
		} else if (rc == 0) {
			if (n == 1)
				return 0;
			else
				break;	
		} else {
			if (errno == EINTR)
				continue;
			return -1;	
		}
	}
	*ptr = 0;	
	return n;
}


ssize_t Readline(int fd, void *ptr, size_t maxlen) {
	ssize_t	n = readline(fd, ptr, maxlen);
	if (n == -1)
        std::cerr << "Readline error\n";
	return n;
}

int Accept(int fd, struct sockaddr *sa, socklen_t *salenptr) {
	int	n;
	if ( (n = accept(fd, sa, salenptr)) < 0) {
			std::cerr << "Accept error\n";
	}
	return n;
}

int Connect(int fd, const sockaddr* addr, socklen_t len) {
	int n = connect(fd, addr, len);
	if (n < 0) {
		std::cerr << "Connect error" << std::endl;
	}
	return n;
}

int Select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds,
       struct timeval *timeout) {
	int	n;
	if ( (n = select(nfds, readfds, writefds, exceptfds, timeout)) < 0)
		std::cerr << "Select error\n";
	return n;
}

void Listen(int fd, int backlog) {
	char	*ptr;
	if ( (ptr = getenv("LISTENQ")) != NULL)
		backlog = atoi(ptr);
	if (listen(fd, backlog) < 0)
		std::cerr << "Listen error\n";
}

void Bind(int fd, const struct sockaddr *sa, socklen_t salen) {
	if (bind(fd, sa, salen) < 0) {
		std::cerr << "BIND ERROR.\nWait 30-60 seconds or change the port/address, the port is busy.\n";
		exit(-1);
	}
}

int Socket(int family, int type, int protocol) {
	int	n;
	if ( (n = socket(family, type, protocol)) < 0)
		std::cerr << "Socket error\n";
	return n;
}

ssize_t Recvfrom(int sockfd, void *buf, size_t nbytes,
			 int flags, sockaddr *from, socklen_t addrlen) {
	socklen_t len = addrlen;
	ssize_t n  = recvfrom(sockfd, buf, nbytes, flags, from, &len);
	if (n == -1){
		std::cerr << "Recvfrom error\n";
	}
	return n;	
}

ssize_t Sendto(int sockfd, void *buf, size_t nbytes,
			 int flags, sockaddr *to, socklen_t addrlen) {
	ssize_t n = sendto(sockfd, buf, nbytes, flags, to, addrlen);
	if (n == -1){
		std::cerr << "Sendto error\n";
	}
	return n;	
}
