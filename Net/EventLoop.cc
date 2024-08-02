#include "EventLoop.h"
#include "../Log/mars_logger.h"
#include <cassert>
#include <poll.h>

__thread EventLoop* t_loopInThisThread = nullptr;

EventLoop::EventLoop() : m_looping(false), m_threadId(gettid()) {
    LogTrace("EventLoop created {} in thread {}", (void*)this, m_threadId);
    if (t_loopInThisThread) {
        LogFatal("Another EventLoop {} exists in this thread {}", (void*)t_loopInThisThread, m_threadId);
        exit(1);
    } else {
        t_loopInThisThread = this;
    }
}

EventLoop::~EventLoop() {
    assert(!m_looping);
    t_loopInThisThread = nullptr;
}

EventLoop* EventLoop::getEventLoopOfCurrentThread() {
    return t_loopInThisThread;
}

void EventLoop::abortNotInLoopThread() {
    LogFatal("EventLoop::abortNotInLoopThread - EventLoop was created in threadId = {}, current thread id = {}",
            m_threadId, gettid());
    exit(1);
}

void EventLoop::loop() {
    assert(!m_looping);
    assertInLoopThread();
    m_looping = true;
    
    ::poll(nullptr, 0, 5*1000);// 相当于休眠5s

    LogTrace("EventLoop stop looping");

    m_looping = false;
}
