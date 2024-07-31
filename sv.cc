#include "InetAddress.h"
#include "Socket.h"
#include <unistd.h>

#define SERV_PORT 1234
#define MAXLINE 128
int main(int argc, char **argv)
{
    char recvMsg[MAXLINE] = {0};

    InetAddress addr("0.0.0.0", SERV_PORT);
    Socket s(socket(AF_INET, SOCK_STREAM, 0));

    //绑定指定ip和端口
    s.bind(addr);

    s.listen();
  
    int connfd = s.accept(&addr);

    
    //读取客户端发送的消息
    if(0 != read(connfd, recvMsg, MAXLINE))
    {
        printf("recv msg:%s\n",recvMsg);
    }

    //将读取内容原封不动地发送回去
    write(connfd, recvMsg, MAXLINE);
    return 0;
}
