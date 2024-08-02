// Created by mars on 2/8/24.

#ifndef MARS_NET_EVENTLOOP_H
#define MARS_NET_EVENTLOOP_H

#include "../Base/noncopyable.h"
#include "../Base/Timestamp.h"
#include "Callbacks.h"
#include "TimerId.h"

#include <thread>
#include <sys/syscall.h>
#include <unistd.h>
#include <memory>
#include <vector>
#include <functional>

namespace mars {
namespace net {

class Channel;
class Poller;
class TimerId;
class TimerQueue;

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

    TimerId runAt(const Timestamp& time, const TimerCallback& cb);
    TimerId runAfter(double delay, const TimerCallback& cb);
    TimerId runEvery(double interval, const TimerCallback& cb);

private:
    void abortNotInLoopThread();

    typedef std::vector<Channel*> ChannelList;

    bool m_looping;
    bool m_quit;
    const pid_t m_threadId;
    std::unique_ptr<Poller> m_poller;
    std::unique_ptr<TimerQueue> m_timerQueue;
    ChannelList m_activeChannels;
};

} // namespace net
} // namespace mars

#endif // MARS_NET_EVENTLOOP_H
