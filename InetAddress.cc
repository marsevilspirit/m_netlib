#include "InetAddress.h"
#include "endian.h"
#include <arpa/inet.h> // inet_pton

InetAddress::InetAddress(uint16_t port, bool loopback) {
    memset(&m_addr, 0, sizeof(this->m_addr));
    m_addr.sin_family = AF_INET;
    m_addr.sin_addr.s_addr = loopback ? INADDR_LOOPBACK : INADDR_ANY;
    m_addr.sin_port = hostToNetwork16(port);
}

InetAddress::InetAddress(const std::string& ip, uint16_t port) {
    memset(&m_addr, 0, sizeof(m_addr));
    m_addr.sin_family = AF_INET;
    inet_pton(AF_INET, ip.c_str(), &m_addr.sin_addr);
    m_addr.sin_port = hostToNetwork16(port);
}

InetAddress::InetAddress(const struct sockaddr_in& addr) : m_addr(addr) 
{
}


const struct sockaddr *InetAddress::getSockAddr() const {
    return (const struct sockaddr*)&m_addr;
}

void InetAddress::setSockAddr(struct sockaddr_in& addr) {
    m_addr = addr;
}

std::string InetAddress::getIpStr() const {
    char buf[64] = "";
    inet_ntop(AF_INET, &m_addr.sin_addr, buf, sizeof(buf));
    return buf;
}
