/*************************************************************************
	> File Name: demo_server.c
	> Author: 师毅
	> Mail:  shiyi19960604@gmail.com
	> Created Time: 2015年08月02日 星期日 14时24分17秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#define PORT_NUM 4507
#define LISTEN_NUM 20


int main()
{
    pid_t pid;
    int sock_fd, conn_fd;
    int optval;
    int cli_len;
    char recv_buf[128],send_buf[128];


    struct sockaddr_in cli_addr,serv_addr;
    
    //创建套接字
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_fd < 0)
    {
        perror("socket");
        exit(1);
    }

    //设置套接字
    optval = 1;
    if(setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR,(void*)&optval,sizeof(int)) < 0)
    {
        perror("setsockopt");
        exit(1);
    }
    
    //初始化
    memset(&serv_addr,0,sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT_NUM);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    //绑定端口
    if(bind(sock_fd,(struct sockaddr*)&serv_addr,sizeof(struct sockaddr)) < 0)
    {
        perror("bind");
        exit(1);
    }

    if(listen(sock_fd, LISTEN_NUM) < 0)
    {
        perror("listen");
        exit(1);
    }
    
    cli_len = sizeof(struct sockaddr_in);
    conn_fd = accept(sock_fd, (struct sockaddr*)&cli_addr, &cli_len);
    
    if(conn_fd < 0)
    {
        perror("accept");
        exit(1);
    }

    printf("accept a new client : %s\n",inet_ntoa(cli_addr.sin_addr));
    
    while(1)
    {
        recv(conn_fd,recv_buf,sizeof(recv_buf),0);
        printf("he:%s\n",recv_buf);
        memset(send_buf,0,sizeof(send_buf));
        printf("me:");
        scanf("%s",send_buf);
        send(conn_fd,send_buf,sizeof(send_buf),0);
    }
}

