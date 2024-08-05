#include "TcpConnection.h"
#include "Channel.h"
#include "../Log/mars_logger.h"
#include "../Socket/SocketOps.h"

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
    m_channel->setCloseCallback(std::bind(&TcpConnection::handleClose, this));
    m_channel->setErrorCallback(std::bind(&TcpConnection::handleError, this));
}

TcpConnection::~TcpConnection(){
    LogInfo("TcpConnection::dtor[{}] at {} fd = {}", m_name, m_localAddr.toHostPort(), m_channel->fd());
}

void TcpConnection::connectDestroyed(){
    m_loop->assertInLoopThread();

    LogTrace("m_state = {}", static_cast<int>(m_state));

    assert(m_state == KConnected);
    setState(KDisconnected);
    m_channel->disableAll();
    m_connectionCallback(shared_from_this());

    m_loop->removeChannel(m_channel.get());
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
        handleClose();
    } else {
        handleError();
    }
}

void TcpConnection::handleClose(){
    m_loop->assertInLoopThread();
    assert(m_state == KConnected);
    LogInfo("enter TcpConnection::handleClose [{}] - SO_ERROR = 0", m_name);
    m_channel->disableAll();
    LogTrace("disableAll() done");
    m_closeCallback(shared_from_this());
    LogTrace("closeCallback() done");
}

void TcpConnection::handleError(){
    int err = sockets::getSocketError(m_channel->fd());
    LogError("TcpConnection::handleError [{}] - SO_ERROR = {} {}", m_name, err, strerror(err));
}
