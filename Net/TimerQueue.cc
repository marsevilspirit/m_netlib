#include "TimerQueue.h"
#include "EventLoop.h"
#include "Timer.h"
#include "TimerId.h"
#include "../Log/mars_logger.h"

#include <sys/timerfd.h>
#include <unistd.h>
#include <assert.h>

namespace mars{
namespace net{
namespace detail{

int createTimerfd(){
    int timerfd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    if(timerfd < 0){
        LogFatal("Failed in timerfd_create");
    }
    return timerfd;
}

struct timespec howMuchTimeFromNow(Timestamp when){
    int64_t microseconds = when.microSecondsSinceEpoch() - Timestamp::now().microSecondsSinceEpoch();
    if(microseconds < 100){
        microseconds = 100;
    }

    struct timespec ts;
    ts.tv_sec = static_cast<time_t>(microseconds / Timestamp::kMicroSecondsPerSecond);
    ts.tv_nsec = static_cast<long>((microseconds % Timestamp::kMicroSecondsPerSecond) * 1000);
    return ts;
}

void readTimerfd(int timerfd, Timestamp now){// 读取 timerfd 的数据, 以免一直触发
    uint64_t howmany;
    ssize_t n = ::read(timerfd, &howmany, sizeof(howmany));
    LogTrace("TimerQueue::readTimerfd() {} at {}", howmany, now.toString().c_str());
    if(n != sizeof(howmany)){
        LogError("TimerQueue::readTimerfd() reads %lu bytes instead of 8", n);
    }
}

void resetTimerfd(int timerfd, Timestamp expiration){
    struct itimerspec newValue;
    struct itimerspec oldValue;
    bzero(&newValue, sizeof(newValue));
    bzero(&oldValue, sizeof(oldValue));
    newValue.it_value = howMuchTimeFromNow(expiration);
    int ret = ::timerfd_settime(timerfd, 0, &newValue, &oldValue);
    if(ret){
        LogError("timerfd_settime()");
    }
}

} // namespace detail
} // namespace net
} // namespace mars

using namespace mars;
using namespace mars::net;
using namespace mars::net::detail;

TimerQueue::TimerQueue(EventLoop* loop)
  : m_loop(loop),
    m_timerfd(createTimerfd()),
    m_timerfdChannel(loop, m_timerfd),
    m_timers()
{
    m_timerfdChannel.setReadCallback(std::bind(&TimerQueue::handleRead, this));
    m_timerfdChannel.enableReading();
}

TimerId TimerQueue::addTimer(const TimerCallback cb, Timestamp when, double interval){
    Timer* timer = new Timer(cb, when, interval);

    m_loop->runInLoop(std::bind(&TimerQueue::addTimerInLoop, this, timer));

    return TimerId(timer);
}

void TimerQueue::addTimerInLoop(Timer* timer) {
    m_loop->assertInLoopThread();
    bool earliestChanged = insert(timer);

    if(earliestChanged){
        resetTimerfd(m_timerfd, timer->expiration());
    }
}

void TimerQueue::handleRead(){
    Timestamp now(Timestamp::now());
    readTimerfd(m_timerfd, now);

    std::vector<Entry> expired = getExpired(now);

    for(auto& entry : expired){
        entry.second->run();
    }

    reset(expired, now);
}

std::vector<TimerQueue::Entry> TimerQueue::getExpired(Timestamp now){
    std::vector<Entry> expired;
    Entry sentry(now, reinterpret_cast<Timer*>(UINTPTR_MAX));
    auto it = m_timers.lower_bound(sentry);
    assert(it == m_timers.end() || now < it->first);
    std::copy(m_timers.begin(), it, std::back_inserter(expired));
    m_timers.erase(m_timers.begin(), it);
    return expired;
}

void TimerQueue::reset(const std::vector<Entry>& expired, Timestamp now){
    Timestamp nextExpire;

    for(auto& entry : expired){
        if(entry.second->repeat()){
            entry.second->restart(now);
            insert(entry.second);
        } else {
            delete entry.second;
        }
    }

    if(!m_timers.empty()){
        nextExpire = m_timers.begin()->second->expiration();
    }

    if(nextExpire.valid()){
        resetTimerfd(m_timerfd, nextExpire);
    }
}

bool TimerQueue::insert(Timer* timer){
    m_loop->assertInLoopThread();

    bool earliestChanged = false;
    Timestamp when = timer->expiration();

    auto it = m_timers.begin();
    if(it == m_timers.end() || when < it->first){
        earliestChanged = true;
    }

    m_timers.insert(Entry(when, std::move(timer)));
    return earliestChanged;
}

TimerQueue::~TimerQueue(){
    ::close(m_timerfd);
    for(auto& timer : m_timers){
        delete timer.second;
    }
}
