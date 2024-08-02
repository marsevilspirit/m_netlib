//Created by mars on 2/8/24

#ifndef MARS_NET_TIMERID_H
#define MARS_NET_TIMERID_H

#include "../Base/copyable.h"
#include <cstdint>

namespace mars{
namespace net{

class Timer;

class TimerId : public base::copyable{
public:
    explicit TimerId(Timer* timer) : m_timer(timer){}

private:
    Timer* m_timer;
};

}
}

#endif // MARS_NET_TIMERID_H
