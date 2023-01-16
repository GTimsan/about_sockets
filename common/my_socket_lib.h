#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <poll.h>
#include <string.h>

// проверить на константность
// комменты для всего
// delete comments
// в клиенте в двух функциях передачи неправильно указан буфер
// сделать абстрактный класс
// getline?

#define OPEN_MAX 256
#define MAXLINE 130
#define SERV_PORT 9999
#define LISTENQ 256
#define INFTIM -1


int Socket(int family, int type, int protocol);
void Bind(int fd, const struct sockaddr *sa, socklen_t salen);
void Listen(int fd, int backlog);
int Poll(pollfd *fdarray, unsigned long nfds, int timeout);
void Shutdown(int fd, int how);
int Select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds,
                            struct timeval *timeout);
int Accept(int fd, struct sockaddr *sa, socklen_t *salenptr);
int Connect(int fd, const sockaddr *addr, socklen_t len);
void Close(int fd);
int Inet_pton(int af, const char* address, void* buf);
void Fputs(const char *ptr, FILE *stream);
ssize_t Writen(int fd, const void *ptr, size_t nbytes);
ssize_t writen(int fd, const void *vptr, size_t n);
ssize_t Readline(int fd, void *ptr, size_t maxlen);
ssize_t readline(int fd, void *vptr, size_t maxlen);
char* Fgets(char *ptr, int n, FILE *stream);
ssize_t Recvfrom(int sockfd, void *buf, size_t nbytes, int flags,
                             sockaddr *from, socklen_t addrlen);
ssize_t Sendto(int sockfd, void *buf, size_t nbytes, 
              int flags, sockaddr *to, socklen_t addrlen);