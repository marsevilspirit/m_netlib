#include "TcpServer.h"
#include "../Log/mars_logger.h"
#include "../Socket/SocketOps.h"

using namespace mars;
using namespace mars::net;

TcpServer::TcpServer(EventLoop* loop, const InetAddress& listenAddr)
    : m_loop(loop),
      m_name(listenAddr.toHostPort()),
      m_acceptor(new Acceptor(loop, listenAddr)),
      m_connectionCallback(),
      m_messageCallback(),
      m_started(false),
      m_nextConnId(1)
{
    m_acceptor->setNewConnectionCallback(std::bind(&TcpServer::newConnection, this, std::placeholders::_1, std::placeholders::_2));
}

TcpServer::~TcpServer(){
}

void TcpServer::start(){
    if (!m_started){
        m_started = true;
    }

    if (!m_acceptor->listenning()){
        m_loop->runInLoop(std::bind(&Acceptor::listen, m_acceptor.get()));
    
    }
}


void TcpServer::newConnection(int sockfd, const InetAddress& peerAddr){
    m_loop->assertInLoopThread();
    char buf[32];
    snprintf(buf, sizeof(buf), "#%d", m_nextConnId);
    ++m_nextConnId;
    std::string connName = m_name + buf;

    LogInfo("TcpServer::newConnection [{}] - new connection [{}] from {}",
            m_name, connName, peerAddr.toHostPort());

    InetAddress localAddr(sockets::getLocalAddr(sockfd));

    TcpConnectionPtr conn(new TcpConnection(m_loop, connName, sockfd, localAddr, peerAddr));
    m_connections[connName] = conn;
    conn->setConnectionCallback(m_connectionCallback);
    conn->setMessageCallback(m_messageCallback);
    conn->connectEstablished();
}
