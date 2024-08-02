#include "Timer.h"

using namespace mars;
using namespace mars::net;

void Timer::restart(Timestamp now){
    if(m_repeat){
        m_expiration = addTime(now, m_interval);
    }else{
        m_expiration = Timestamp::invalid();
    }
}
