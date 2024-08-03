#include "TcpConnection.h"
#include "Channel.h"
#include "../Log/mars_logger.h"

#include <assert.h>

using namespace mars;
using namespace mars::net;

TcpConnection::TcpConnection(EventLoop* loop, const std::string& name, int sockfd, const InetAddress& localAddr, const InetAddress& peerAddr)
    : m_loop(loop),
      m_name(name),
      m_state(kConnecting),
      m_socket(new Socket(sockfd)),
      m_channel(new Channel(loop, sockfd)),
      m_localAddr(localAddr),
      m_peerAddr(peerAddr)
{
    LogInfo("TcpConnection::ctor[{}] at {} fd = {}", m_name, m_localAddr.toHostPort(), sockfd);
    m_channel->setReadCallback(std::bind(&TcpConnection::handleRead, this));
}

TcpConnection::~TcpConnection(){
    LogInfo("TcpConnection::dtor[{}] at {} fd = {}", m_name, m_localAddr.toHostPort(), m_channel->fd());
}

void TcpConnection::connectEstablished(){
    m_loop->assertInLoopThread();
    assert(m_state == kConnecting);
    setState(KConnected);
    m_channel->enableReading();
    m_connectionCallback(shared_from_this());
}

void TcpConnection::handleRead(){
    char buf[65536];
    ssize_t n = ::read(m_channel->fd(), buf, sizeof(buf));

    if (n > 0){
        m_messageCallback(shared_from_this(), buf, n);
    }
    else if (n == 0){
        ::close(m_channel->fd());
        LogInfo("TcpConnection::handleRead - connection [{}] is down", m_name);
    } else {
        ::close(m_channel->fd());
        LogError("TcpConnection::handleRead - connection [{}] error", m_name);
    }
}
