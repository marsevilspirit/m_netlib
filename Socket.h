#ifndef SOCKET_H
#define SOCKET_H

#include "InetAddress.h"

class Socket {
public:
    explicit Socket(int sockfd);
    ~Socket();

    int fd() const { return m_sockfd; }

    void bind(const InetAddress& localaddr);
    void listen() const;
    int accept(InetAddress* peeraddr);

    void shutdownWrite();

    void setTcpNoDelay(bool on);
    void setKeepAlive(bool on);
    void setReuseAddr(bool on) const;
    void setReusePort(bool on) const;

private:
    const int m_sockfd;
};

#endif // SOCKET_H
