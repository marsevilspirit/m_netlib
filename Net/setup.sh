rm a.out
g++ test.cc Channel.cc EventLoop.cc Poller.cc Timer.cc TimerQueue.cc ../Log/mars_logger.cc ../Base/Timestamp.cc EventLoopThread.cc ../Socket/InetAddress.cc ../Socket/Socket.cc ../Socket/SocketOps.cc Acceptor.cc TcpServer.cc TcpConnection.cc -lfmt -ljsoncpp -g
./a.out
