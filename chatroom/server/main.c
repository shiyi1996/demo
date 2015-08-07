/*************************************************************************
	> File Name: 1.c
	> Author: 师毅
	> Mail:  shiyi19960604@gmail.com
	> Created Time: 2015年08月07日 星期五 08时41分20秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>
#include<sys/socket.h>
#include<sys/epoll.h>
#include<fcntl.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<pthread.h>
#include"user.h"



#define LISTENMAX 200
#define PORT 8001
#define IP_ADDR "192.168.20.145"
#define EPOLL_MAX 2000

int f[2001] = {0};

struct Pthread
{
	int fd;
	new_t new;
};

void *def(struct Pthread *data)
{
	int conn_fd;
	new_t send_new;
	user_t user;
	int temp_id;
	
	conn_fd = data->fd;
	switch (data->new.type)
	{	
		case 1://注册
		
			if(find_user_id(data->new.user.name, &user) != 0)
			{
				strcpy(send_new.buf, "！！！用户名已存在");
			}
			else
			{
				if(add_user(&data->new.user) == 1)
				{
					strcpy(send_new.buf, "注册成功");
				}
			}
			send(conn_fd, &send_new, sizeof(new_t), 0);
			break;
		
		case 2://登陆
		
			if(find_user_id(data->new.user.name, &user) == 0)
			{
				strcpy(send_new.buf, "！！！用户名不存在");
			}
			else
			{
				if(strcmp(data->new.user.pass, user.pass) == 0)
				{
					if(f[user.id]==0)
					{
						strcpy(send_new.buf, "登陆成功");
						printf("%s 上线了！\n",user.name);
						f[user.id] = conn_fd;
					}
					else
					{
						strcpy(send_new.buf, "！！！账号已在线");
					}
				}
				else
				{
					strcpy(send_new.buf, "！！！密码错误");
				}
			}
			send(conn_fd, &send_new, sizeof(new_t), 0);
			break;
		
		case 3://私聊
			
			temp_id = find_user_id(data->new.to_name, &user);
			if(temp_id == 0)//好友名不存在，通知用户
			{
				if(send(conn_fd, &data->new, sizeof(new_t), 0) < 0)
				{
					perror("send");
				}
			}
			else
			{
				while(1)
				{
					if(f[temp_id])
					{
						if(send(f[temp_id], &data->new, sizeof(new_t), 0) < 0)
						{
							perror("send");
						}
						break;
					}
				}
			}
			break;
		case 4://群发
			
			break;
			
		case 5://查询在线用户
		
			break;
			
		case 6://查询所有用户
		
			break;
			
		default:
			break;
			
	}

	return NULL;
}

int main()
{
	struct epoll_event ev, events[LISTENMAX];
	int epfd, listen_fd, nfds, sock_fd;
	struct sockaddr_in addr;
	socklen_t addr_len;
	int n, i, j;
	pthread_t thread;
	new_t new;
	struct Pthread *data;
	user_t user;
	
	epfd = epoll_create(EPOLL_MAX+1);
	listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	i=1;
	setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &i, sizeof(int));
	ev.data.fd = listen_fd;
	ev.events = EPOLLIN;
	epoll_ctl(epfd, EPOLL_CTL_ADD, listen_fd, &ev);
		
	memset(&addr, 0, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);	
	addr.sin_port = htons(PORT);

	if(bind(listen_fd, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)) < 0)
	{
		perror("bind");
		exit(1);
	}
	
	listen(listen_fd, LISTENMAX);
	
	addr_len = sizeof(struct sockaddr_in);
	
	while(1)
	{
		nfds = epoll_wait(epfd, events, EPOLL_MAX, 1000);
		
		for(i=0; i<nfds; i++)
		{
			if(events[i].data.fd == listen_fd)
			{
				sock_fd = accept(listen_fd, (struct sockaddr*)&addr,&addr_len);
				printf("accept a new client: %s\n",inet_ntoa(addr.sin_addr));
				ev.data.fd = sock_fd;
				ev.events = EPOLLIN;
				epoll_ctl(epfd, EPOLL_CTL_ADD, sock_fd, &ev);
			}
			
			else if(events[i].events & EPOLLIN)
			{
				n = recv(events[i].data.fd, &new, sizeof(struct News), 0);
				
				if(n < 0)
				{
					close(events[i].data.fd);
					perror("recv");
					continue;
				}
				else if(n == 0)
				{
					ev.data.fd = events[i].data.fd;
					epoll_ctl(epfd, EPOLL_CTL_DEL, events[i].data.fd, &ev);			
					for(j=1;j<=2000;j++)
					{
						if(f[j] == events[i].data.fd)
						{
							break;
						}
					}
					if(j <= 2000)
					{
						f[j] = 0;
						find_user_name(j, &user);
						printf("%s 下线了！\n",user.name);
					}

					close(events[i].data.fd);
					
					continue;
				}
				
				data = (struct Pthread*)malloc(sizeof(struct Pthread));
				data->fd = events[i].data.fd;
				memcpy(&data->new, &new, sizeof(new_t));
				pthread_create(&thread, NULL, (void*)def, data);
			}
		
		}
	}
	
}

