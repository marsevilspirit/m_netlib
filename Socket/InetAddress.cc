#include "InetAddress.h"
#include <arpa/inet.h> // inet_pton

InetAddress::InetAddress(uint16_t port, const std::string& ip) {
    memset(&m_addr, 0, sizeof(m_addr));
    m_addr.sin_family = AF_INET;
    inet_pton(AF_INET, ip.c_str(), &m_addr.sin_addr);
    m_addr.sin_port = htons(port);
}

std::string InetAddress::getIpStr() const {
    char buf[64] = {0};
    inet_ntop(AF_INET, &m_addr.sin_addr, buf, sizeof(buf));
    return buf;
}

std::string InetAddress::getPortStr() const {
    return std::to_string(ntohs(m_addr.sin_port));
}

std::string InetAddress::getIpPortStr() const {
    return getIpStr() + ":" + getPortStr();
}
