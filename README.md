# m_netlib

这是我学习《Linux多线程服务端编程：使用muduo C++网络库》这本书所写的一个简易的muduo网络库。目前只能编写最基本的TCP高并发服务器，很多处理异常情况的方法都没有编写。

#### 模块

- Base模块
- Log模块
- Net模块
- Socket模块



#### Net模块



##### Channel类

Channel类用于管理一个文件描述符，它将文件描述符及该文件描述符对应的回调函数绑定在了一起。

一个Channel只属于一个eventloop，只管理一个fd。

回调函数：

```c++
void setReadCallback(ReadEventCallback cb) {read_callback_ = std::move(cb);}
void setWriteCallback(Eventcallback cb) {write_callback_ = std::move(cb);}
void setCloseCallback(EventCallback cb) {close_callback_ = std::move(cb);}
void setErrorCallback(EventCallback cb) {error_callback_ = std::move(cb);}
```



##### EPoller类

负责监听文件描述符事件是否触发以及返回发生事件的文件描述符以及具体事件的模块就是EPoller。所以一个EPoller对象对应一个事件监听器。在Multi-Reactor模型中，有多少Reactor就有多少EPoller。muduo提供了epoll和poll两种IO多路复用方法来实现事件监听。

一个EPoller只属于一个eventloop，一个EPoller管理很多的Channel。



##### EventLoop类

EventLoop就是负责实现“循环”，负责驱动“循环”的重要模块！Channel和Poller其实相当于EventLoop的手下，EventLoop整合封装了二者并向上提供了更方便的接口来使用。

EventLoop核心逻辑：

```c++
void EventLoop::loop() {
    while (!m_quit) {
        m_activeChannels.clear();
        m_pollReturnTime = m_poller->poll(kPollTimeMs, &m_activeChannels);
        for(auto& ch : m_activeChannels) {
            ch->handleEvent(m_pollReturnTime);
        }
        doPendingFunctors();
    }

    LogTrace("EventLoop {} stop looping", (void*)this);
}
```

