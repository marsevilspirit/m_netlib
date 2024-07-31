#include "SocketOps.h"
#include "mars_logger.h"

void sockets::bind(int sockfd, const struct sockaddr* addr)
{
    if (::bind(sockfd, addr, static_cast<socklen_t>(sizeof(struct sockaddr_in6))) < 0)
    {
        LogError("bind error");
    }
}

void sockets::listen(int sockfd)
{
    if (::listen(sockfd, SOMAXCONN) < 0)
    {
        LogError("listen error");
    }
}

int sockets::accept(int sockfd, struct sockaddr_in* addr)
{
    socklen_t addrlen = static_cast<socklen_t>(sizeof(*addr));
    int connfd = ::accept(sockfd, reinterpret_cast<struct sockaddr*>(addr), &addrlen);
    if (connfd < 0)
    {
        LogError("accept error");
    }
    return connfd;
}

ssize_t sockets::read(int sockfd, void *buf, size_t count) {
    return ::read(sockfd, buf, count);
}

ssize_t sockets::write(int sockfd, const void *buf, size_t count) {
    return ::write(sockfd, buf, count);
}

void sockets::close(int sockfd) {
    int ret = ::close(sockfd);
    if (ret < 0) {
        LogError("Close error!");
    }
}
