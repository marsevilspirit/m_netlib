#include "InetAddress.h"
#include "endian.h"
#include <arpa/inet.h> // inet_pton

InetAddress::InetAddress(uint16_t port, bool loopback, bool ipv6) {
    if (ipv6) {
        memset(&m_addr6, 0, sizeof(this->m_addr6));
        m_addr6.sin6_family = AF_INET6;
        m_addr6.sin6_addr = loopback ? in6addr_loopback : in6addr_any;
        m_addr6.sin6_port = hostToNetwork16(port);
    } else {
        memset(&m_addr, 0, sizeof(this->m_addr));
        m_addr.sin_family = AF_INET;
        m_addr.sin_addr.s_addr = loopback ? INADDR_LOOPBACK : INADDR_ANY;
        m_addr.sin_port = hostToNetwork16(port);
    }
}

InetAddress::InetAddress(const std::string& ip, uint16_t port, bool ipv6) {
    if (ipv6) {
        memset(&m_addr6, 0, sizeof(this->m_addr6));
        m_addr6.sin6_family = AF_INET6;
        inet_pton(AF_INET6, ip.c_str(), &m_addr6.sin6_addr);
        m_addr6.sin6_port = hostToNetwork16(port);
    } else {
        memset(&m_addr, 0, sizeof(this->m_addr));
    }
}

InetAddress::InetAddress(const struct sockaddr_in& addr) : m_addr(addr) 
{
}

InetAddress::InetAddress(const struct sockaddr_in6& addr) : m_addr6(addr) 
{
}

std::string InetAddress::getIpStr() const {
    char buf[64] = "";
    if (m_addr.sin_family == AF_INET) {
        inet_ntop(AF_INET, &m_addr.sin_addr, buf, sizeof(buf));
    } else {
        inet_ntop(AF_INET6, &m_addr6.sin6_addr, buf, sizeof(buf));
    }
    return buf;
}
