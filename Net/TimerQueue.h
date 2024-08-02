//Created by mars on 2/8/24

#ifndef MARS_NET_TIMERQUEUE_H
#define MARS_NET_TIMERQUEUE_H

#include "../Base/noncopyable.h"
#include "Callbacks.h"
#include "../Base/Timestamp.h"
#include "Channel.h"

#include <memory>
#include <set>

namespace mars{
namespace net{

class EventLoop;
class Timer;
class TimerId;

class TimerQueue : noncopyable{
public:
    TimerQueue(EventLoop* loop);
    ~TimerQueue();

    TimerId addTimer(const TimerCallback cb, Timestamp when, double interval);

private:
    typedef std::pair<Timestamp, Timer*> Entry;
    typedef std::set<Entry> TimerList;

    void handleRead();
    std::vector<Entry> getExpired(Timestamp now);
    void reset(const std::vector<Entry>& expired, Timestamp now);

    bool insert(Timer* timer);

    EventLoop* m_loop;
    const int m_timerfd;
    Channel m_timerfdChannel;
    TimerList m_timers;
};

}
}

#endif // MARS_NET_TIMERQUEUE_H
