//Created by mars on 3/8/24

#ifndef MARS_NET_TCP_SERVER_H
#define MARS_NET_TCP_SERVER_H

#include "../Base/noncopyable.h"
#include "../Socket/InetAddress.h"
#include "EventLoop.h"
#include "Acceptor.h"
#include "TcpConnection.h"

#include <map>

namespace mars{
namespace net{

class TcpServer : noncopyable{
public:
    TcpServer(EventLoop* loop, const InetAddress& listenAddr);
    ~TcpServer();

    void start();

    void setConnectionCallback(const ConnectionCallback& cb){ m_connectionCallback = cb;}

    void setMessageCallback(const MessageCallback& cb){ m_messageCallback = cb;}

private:

    void newConnection(int sockfd, const InetAddress& peerAddr);

    typedef std::map<std::string, TcpConnectionPtr> ConnectionMap;

    EventLoop* m_loop;
    const std::string m_name;
    std::unique_ptr<Acceptor> m_acceptor;
    ConnectionCallback m_connectionCallback;
    MessageCallback m_messageCallback;
    bool m_started;
    int m_nextConnId;
    ConnectionMap m_connections;
};

} //namespace net
} //namespace mars


#endif //MARS_NET_TCP_SERVER_H
