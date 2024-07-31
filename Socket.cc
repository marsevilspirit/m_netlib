#include "Socket.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/uio.h>

Socket::Socket(int sockfd) : m_sockfd(sockfd) 
{
}

Socket::~Socket() 
{
    close(m_sockfd);
    if (m_sockfd < 0) {
        // LOG_SYSERR << "sockets::close";
    }
}
