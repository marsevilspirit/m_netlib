// Created by mars on 2/8/24.

#ifndef MARS_NET_EVENTLOOP_H
#define MARS_NET_EVENTLOOP_H

#include "../Base/noncopyable.h"

#include <thread>
#include <sys/syscall.h>
#include <unistd.h>
#include <memory>
#include <vector>

namespace mars {
namespace net {

class Channel;
class Poller;

class EventLoop : noncopyable {
public:
    EventLoop();
    ~EventLoop();

    void loop();

    void quit();

    void assertInLoopThread() {
        if (!isInLoopThread()) {
            abortNotInLoopThread();
        }
    }

    bool isInLoopThread() const { return m_threadId == gettid(); }

    static EventLoop* getEventLoopOfCurrentThread();

    void updateChannel(Channel* channel);

private:
    void abortNotInLoopThread();

    typedef std::vector<Channel*> ChannelList;

    bool m_looping;
    bool m_quit;
    const pid_t m_threadId;
    std::unique_ptr<Poller> m_poller;
    ChannelList m_activeChannels;
};

} // namespace net
} // namespace mars

#endif // MARS_NET_EVENTLOOP_H
