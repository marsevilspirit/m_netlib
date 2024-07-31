#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include <arpa/inet.h>
#include<sys/socket.h>
#include<sys/types.h>
#define MAXLINE 128
int main(int argc, char **argv)
{
    int    sockfd;  //连接描述符
    struct sockaddr_in    servaddr;//socket结构信息
    char sendMsg[MAXLINE] = {0};
    char recvMsg[MAXLINE] = {0};
    
    //检查参数数量
    if (argc < 2)
    {
        printf("usage: ./client ip port\n");
        return -1;
    }
    //初始化结构体
    bzero(&servaddr, sizeof(servaddr));
    
    //指定协议族
    servaddr.sin_family = AF_INET;
    //第一个参数为ip地址，需要把ip地址转换为sin_addr类型
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    //第二个参数为端口号
    servaddr.sin_port = htons(atoi(argv[2]));

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == sockfd)
    {
        perror("socket error");
        return -1;
    }

    //连接服务器，如果非0，则连接失败
    if(0 != connect(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)))
    {
        perror("connect failed");
        return -1;
    }
    
    //从控制台读取消息
    if(NULL !=fgets(sendMsg,MAXLINE,stdin))
    {        
        write(sockfd, sendMsg, strlen(sendMsg));
    }
    if(0 != read(sockfd, recvMsg, MAXLINE))
    {
        printf("recv msg:%s\n",recvMsg);
    }
    close(sockfd);
    return 0;
}
