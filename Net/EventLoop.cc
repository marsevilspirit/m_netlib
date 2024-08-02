#include "EventLoop.h"
#include "../Log/mars_logger.h"
#include "Poller.h"
#include "Channel.h"
#include <assert.h>

using namespace mars::net;

__thread EventLoop* t_loopInThisThread = nullptr;

const int kPollTimeMs = 10000;

EventLoop::EventLoop() 
  : m_looping(false), 
    m_threadId(gettid()), 
    m_poller(new Poller(this)), 
    m_quit(false)
{
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
    m_quit = false;
    
    while (!m_quit) {
        m_activeChannels.clear();
        m_poller->poll(kPollTimeMs, &m_activeChannels);
        for(auto ch : m_activeChannels) {
            ch->handleEvent();
        }
    }

    LogTrace("EventLoop {} stop looping", (void*)this);
    m_looping = false;
}

void EventLoop::quit() {
    m_quit = true;
}

void EventLoop::updateChannel(Channel* channel) {
    assert(channel->ownerLoop() == this);
    assertInLoopThread();
    m_poller->updateChannel(channel);
}
