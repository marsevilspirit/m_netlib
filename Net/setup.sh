rm a.out
g++ test1.cc Channel.cc EventLoop.cc EventLoopThreadPool.cc Poller.cc Timer.cc TimerQueue.cc ../Log/mars_logger.cc ../Base/Timestamp.cc EventLoopThread.cc ../Socket/InetAddress.cc ../Socket/Socket.cc ../Socket/SocketOps.cc Acceptor.cc TcpServer.cc TcpConnection.cc Buffer.cc -lfmt -ljsoncpp -g
./a.out
