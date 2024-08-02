#include "Socket.h"
#include "SocketOps.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/tcp.h>

Socket::Socket(int sockfd) : m_sockfd(sockfd) 
{
}

Socket::~Socket() 
{
    sockets::close(m_sockfd);
}

void Socket::bind(const InetAddress& localaddr) 
{
    sockets::bind(m_sockfd, (sockaddr*)localaddr.getSockAddr());
}

void Socket::listen() const 
{
    sockets::listen(m_sockfd);
}

int Socket::accept(InetAddress* peeraddr) 
{
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    int connfd = sockets::accept(m_sockfd, &addr);
    if (connfd >= 0) 
    {
        peeraddr->setSockAddr(addr);
    }
    return connfd;
}

void Socket::setReuseAddr(bool on) const 
{
    int optval = on ? 1 : 0;
    ::setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, static_cast<socklen_t>(sizeof(optval)));
}

void Socket::setReusePort(bool on) const 
{
    int optval = on ? 1 : 0;
    ::setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEPORT, &optval, static_cast<socklen_t>(sizeof(optval)));
}

void Socket::setTcpNoDelay(bool on) {
    int optVal = on ? 1 : 0;
    ::setsockopt(m_sockfd, IPPROTO_TCP, TCP_NODELAY, &optVal, static_cast<socklen_t>(sizeof optVal));
}

void Socket::setKeepAlive(bool on) {
    int optVal = on ? 1 : 0;
    ::setsockopt(m_sockfd, SOL_SOCKET, SO_KEEPALIVE, &optVal, static_cast<socklen_t>(sizeof optVal));
}
