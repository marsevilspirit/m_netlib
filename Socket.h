#ifndef SOCKET_H
#define SOCKET_H

#include "InetAddress.h"

class Socket {
public:
    explicit Socket(int sockfd);
    ~Socket();

    int fd() const { return m_sockfd; }

    void bindAddress(const InetAddress& localaddr);
    void listenAddress() const;
    int acceptAddress(InetAddress* peeraddr);

    void shutdownWrite();

    void setTcpNoDelay(bool on);
    void setReuseAddr(bool on);
    void setReusePort(bool on);
    void setKeepAlive(bool on);

private:
    const int m_sockfd;
};

#endif // SOCKET_H
