//Created by mars on 2/8/24

#ifndef MARS_NET_CALLBACKS_H
#define MARS_NET_CALLBACKS_H

#include "../Base/Timestamp.h"

#include <functional>

namespace mars{

using base::Timestamp;
using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

namespace net{
// All client visible callbacks go here.

typedef std::function<void()> TimerCallback;

}
}

#endif // MARS_NET_CALLBACKS_H
