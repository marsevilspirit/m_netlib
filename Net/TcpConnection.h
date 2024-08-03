//Created by mars on 3/8/24

#ifndef MARS_NET_TCPCONNECTION_H
#define MARS_NET_TCPCONNECTION_H

#include "../Base/noncopyable.h"
#include "EventLoop.h"
#include "../Socket/Socket.h"

#include <memory>

namespace mars {
namespace net {

class TcpConnection : noncopyable, public std::enable_shared_from_this<TcpConnection>{
public:
    TcpConnection(EventLoop* loop, const std::string& name, int sockfd, const InetAddress& localAddr, const InetAddress& peerAddr);
    ~TcpConnection();

    EventLoop* getLoop() const { return m_loop;}
    const std::string& name() const { return m_name;}
    const InetAddress& localAddress() const { return m_localAddr;}
    const InetAddress& peerAddress() const { return m_peerAddr;}
    bool connected() const { return m_state == KConnected;}

    void setConnectionCallback(const ConnectionCallback& cb){ m_connectionCallback = cb;}
    void setMessageCallback(const MessageCallback& cb){ m_messageCallback = cb;}

    void connectEstablished();
private:
    enum StateE { kConnecting, KConnected };

    void setState(StateE s){ m_state = s;}
    void handleRead();

    EventLoop* m_loop;
    std::string m_name;
    StateE m_state;
    std::unique_ptr<Socket> m_socket;
    std::unique_ptr<Channel> m_channel;
    InetAddress m_localAddr;
    InetAddress m_peerAddr;
    ConnectionCallback m_connectionCallback;
    MessageCallback m_messageCallback;
};

} //namespace net
} //namespace mars

#endif //MARS_NET_TCPCONNECTION_H