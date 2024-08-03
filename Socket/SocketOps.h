#ifndef SOCKETOPS_H
#define SOCKETOPS_H

#include <arpa/inet.h>
#include <unistd.h>

namespace mars{
namespace sockets{

    int createNonblockingOrDie();

    void bindOrDie(int sockfd, const struct sockaddr_in& addr);
    void listenOrDie(int sockfd);
    int accept(int sockfd, struct sockaddr_in* addr);
    void close(int sockfd);

    void setNonBlockAndCloseOnExec(int sockfd);

    struct sockaddr_in getLocalAddr(int sockfd);

} // namespace sockets
} // namespace mars

#endif // SOCKETOPS_H
