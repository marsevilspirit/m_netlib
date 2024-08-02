#ifndef INETADDRESS_H
#define INETADDRESS_H

#include <string>
#include <netinet/in.h>
#include <string.h>

class InetAddress {
public:
    InetAddress(uint16_t port, const std::string& ip = "127.0.0.1");

    std::string getIpStr() const;

    std::string getPortStr() const;

    std::string getIpPortStr() const;
    
    const sockaddr_in* getSockAddr() const { return &m_addr; }

private:
    struct sockaddr_in m_addr;
};

#endif // INETADDRESS_H
