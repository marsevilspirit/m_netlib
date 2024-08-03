#include "SocketOps.h"
#include "../Log/mars_logger.h"
#include <fcntl.h>

using namespace mars;

int sockets::createNonblockingOrDie()
{
    int sockfd = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
    if (sockfd < 0)
    {
        LogError("create socket error");
    }
    return sockfd;
}

void sockets::bindOrDie(int sockfd, const struct sockaddr_in& addr)
{
    int ret = ::bind(sockfd, reinterpret_cast<const struct sockaddr*>(&addr), sizeof(addr));
    if (ret < 0)
    {
        LogError("bind error");
    }
}

void sockets::listenOrDie(int sockfd)
{
    if (::listen(sockfd, SOMAXCONN) < 0)
    {
        LogError("listen error");
    }
}

int sockets::accept(int sockfd, struct sockaddr_in* addr) //muduo此处用了accept4
{
    socklen_t addrlen = sizeof(*addr);
    int connfd = ::accept(sockfd, reinterpret_cast<struct sockaddr*>(addr), &addrlen);

    setNonBlockAndCloseOnExec(connfd);

    if (connfd < 0)
    {
        LogError("accept error");
    }
    return connfd;

}

void sockets::close(int sockfd) {
    int ret = ::close(sockfd);
    if (ret < 0) {
        LogError("Close error!");
    }
}

void sockets::setNonBlockAndCloseOnExec(int sockfd) {
    int flags = ::fcntl(sockfd, F_GETFL, 0);
    flags |= O_NONBLOCK;
    int ret = ::fcntl(sockfd, F_SETFL, flags);

    flags = ::fcntl(sockfd, F_GETFD, 0);
    flags |= FD_CLOEXEC;
    ret = ::fcntl(sockfd, F_SETFD, flags);
}

struct sockaddr_in sockets::getLocalAddr(int sockfd) {
    struct sockaddr_in localAddr;
    memset(&localAddr, 0, sizeof(localAddr));
    socklen_t addrlen = sizeof(localAddr);
    if (::getsockname(sockfd, reinterpret_cast<struct sockaddr*>(&localAddr), &addrlen) < 0) {
        LogError("getsockname error");
    }
    return localAddr;
}
