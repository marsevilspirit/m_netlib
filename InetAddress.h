#ifndef INETADDRESS_H
#define INETADDRESS_H

#include <string>
#include <netinet/in.h>
#include <string.h>

class InetAddress {
public:
    explicit InetAddress(uint16_t port = 0, bool loopback = false, bool ipv6 = false);
    InetAddress(const std::string& ip, uint16_t port, bool ipv6 = false);

    explicit InetAddress(const struct sockaddr_in& addr);
    explicit InetAddress(const struct sockaddr_in6& addr);

    std::string getIpStr() const;

private:
    union {
        struct sockaddr_in m_addr;
        struct sockaddr_in6 m_addr6;
    };
};

#endif // INETADDRESS_H
