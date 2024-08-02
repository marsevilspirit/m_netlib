#include "../Base/noncopyable.h"
#include <thread>
#include <sys/syscall.h>
#include <unistd.h>

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
