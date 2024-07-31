#ifndef INETADDRESS_H
#define INETADDRESS_H

#include <string>
#include <netinet/in.h>
#include <string.h>

class InetAddress {
public:
    explicit InetAddress(uint16_t port = 0, bool loopback = false);
    InetAddress(const std::string& ip, uint16_t port);

    explicit InetAddress(const struct sockaddr_in& addr);

    const struct sockaddr* getSockAddr() const;

    void setSockAddr(struct sockaddr_in& addr);

    std::string getIpStr() const;

private:
    struct sockaddr_in m_addr;
};

#endif // INETADDRESS_H
