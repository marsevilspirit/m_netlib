#ifndef SOCKETOPS_H
#define SOCKETOPS_H

#include <arpa/inet.h>
#include <unistd.h>

namespace sockets
{
    void bind (int sockfd, const struct sockaddr* addr);
    void listen(int sockfd);
    int accept(int sockfd, struct sockaddr_in* addr);
    void close(int sockfd);

    ssize_t read(int sockfd, void *buf, size_t count);
    ssize_t write(int sockfd, const void *buf, size_t count);
}

#endif // SOCKETOPS_H
