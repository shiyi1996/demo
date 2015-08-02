/*************************************************************************
	> File Name: demo_client.c
	> Author: 师毅
	> Mail:  shiyi19960604@gmail.com
	> Created Time: 2015年08月02日 星期日 15时42分25秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>

int main(int argc,char *argv[])
{
    int conn_fd;
    struct sockaddr_in serv_addr;
    char send_buf[128],recv_buf[128];

    memset(&serv_addr,0,sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(4507);
    inet_aton(argv[1],&serv_addr.sin_addr);

    conn_fd = socket(AF_INET,SOCK_STREAM,0);
    if(conn_fd < 0)
    {
        perror("socket");
        exit(1);
    }

    if(connect(conn_fd,(struct sockaddr*)&serv_addr,sizeof(struct sockaddr_in)) < 0)
    {
        perror("connect");
        exit(1);
    }

    while(1)
    {
        printf("me:");
        scanf("%s",send_buf);
        if(send(conn_fd,send_buf,sizeof(send_buf),0) < 0)
        {
            perror("send");
            exit(1);
        }
        
        memset(recv_buf,0,sizeof(recv_buf));
        if(recv(conn_fd,recv_buf,sizeof(recv_buf),0) < 0)
        {
            perror("recv");
            exit(1);
        }
        printf("he:%s\n",recv_buf);
    }
}

