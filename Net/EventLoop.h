// Created by mars on 2/8/24.

#ifndef MARS_NET_EVENTLOOP_H
#define MARS_NET_EVENTLOOP_H

#include "../Base/noncopyable.h"
#include <thread>
#include <sys/syscall.h>
#include <unistd.h>

namespace mars {
namespace net {

class EventLoop : noncopyable {
public:
    EventLoop();
    ~EventLoop();

    void loop();

    void assertInLoopThread() {
        if (!isInLoopThread()) {
            abortNotInLoopThread();
        }
    }

    bool isInLoopThread() const { return m_threadId == gettid(); }

    static EventLoop* getEventLoopOfCurrentThread();

private:
    void abortNotInLoopThread();

    bool m_looping;
    const pid_t m_threadId;
};

} // namespace net
} // namespace mars

#endif // MARS_NET_EVENTLOOP_H
