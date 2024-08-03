#include "Poller.h"
#include "Channel.h"
#include "../Log/mars_logger.h"
#include <poll.h>
#include <assert.h>

using namespace mars::net;
using namespace mars::base;

Poller::Poller(EventLoop* loop)
    : m_ownerLoop(loop)
{
}

Poller::~Poller()
{
}

Timestamp Poller::poll(int timeoutMS, ChannelList* activeChannels){
    int numEvents = ::poll(&m_pollfds.front(), m_pollfds.size(), timeoutMS);
    Timestamp now(Timestamp::now());
    if(numEvents > 0){
        LogTrace("{} events happened", numEvents);
        fillActiveChannels(numEvents, activeChannels);
    }else if(numEvents == 0){
        LogTrace("nothing happened");
    }else{
        LogError("Poller::poll()");
    }

    return now;
}

void Poller::fillActiveChannels(int numEvents, ChannelList* activeChannels) const
{
    for(auto pfd = m_pollfds.begin(); pfd != m_pollfds.end() && numEvents > 0; ++pfd){
        if(pfd->revents > 0){
            --numEvents;
            auto ch = m_channels.find(pfd->fd);
            assert(ch != m_channels.end());
            Channel* channel = ch->second;
            channel->set_revents(pfd->revents);
            activeChannels->push_back(channel);
        }
    }
}

void Poller::updateChannel(Channel* channel){
    assertInLoopThread();
    if (channel->index() < 0){
        assert(m_channels.find(channel->fd()) == m_channels.end());
        struct pollfd pfd;
        pfd.fd = channel->fd();
        pfd.events = static_cast<short>(channel->events());
        pfd.revents = 0;
        m_pollfds.push_back(pfd);
        int idx = static_cast<int>(m_pollfds.size()) - 1;
        channel->set_index(idx);
        m_channels[pfd.fd] = channel;
    } else {
        assert(m_channels.find(channel->fd()) != m_channels.end());
        assert(m_channels[channel->fd()] == channel);
        int idx = channel->index();
        assert(0 <= idx && idx < static_cast<int>(m_pollfds.size()));
        struct pollfd& pfd = m_pollfds[idx];
        assert(pfd.fd == channel->fd() || pfd.fd == -channel->fd() - 1);
        pfd.events = static_cast<short>(channel->events());
        pfd.revents = 0;
        if(channel->isNoneEvent()){
            pfd.fd = -channel->fd() - 1;
        }
    }
}
