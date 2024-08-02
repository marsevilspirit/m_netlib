#include "Channel.h"
#include "EventLoop.h"
#include "../Log/mars_logger.h"
#include <poll.h>

using namespace mars::net;

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = POLLIN | POLLPRI;
const int Channel::kWriteEvent = POLLOUT;

Channel::Channel(EventLoop* loop, int fd)
    : m_loop(loop),
      m_fd(fd),
      m_events(0),
      m_revents(0),
      m_index(-1)
{
}

void Channel::update(){
    m_loop->updateChannel(this);// EventLoop.h 中的 updateChannel
}

void Channel::handleEvent(){
    if(m_revents & POLLNVAL){
        LogWarn("Channel::handleEvent() POLLNVAL");// 文件描述符不是一个打开的文件
    }

    if(m_revents & (POLLERR | POLLNVAL)){
        if(m_errorCallback) m_errorCallback();
    }

    if(m_revents & (POLLIN | POLLPRI | POLLRDHUP)){
        if(m_readCallback) m_readCallback();
    }

    if(m_revents & POLLOUT){
        if(m_writeCallback) m_writeCallback();
    }
}
