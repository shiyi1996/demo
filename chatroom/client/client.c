/*************************************************************************
	> File Name: cli.c
	> Author: 师毅
	> Mail:  shiyi19960604@gmail.com
	> Created Time: 2015年08月06日 星期四 01时10分43秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<time.h>
#include"client.h"
#include"common.h"

#define IPADDR "192.168.20.145"
#define PORT_NUM 8001

char user_name[20];

void def(int *conn)
{
	new_t new;
	int conn_fd;
	conn_fd = *conn;
	int ret;
	int file_fd;
	pthread_t thread;
	
	while(1)
	{
		ret = recv(conn_fd, &new, sizeof(new_t), 0);
		if(ret < 0)
		{
			perror("recv");
		}
		else if(ret == 0)
		{
			printf("[错误]服务端意外关闭\n");
			//pthread_exit(0);
			exit(1);
		}

		switch(new.type)
		{
			case 3://私聊
				if(strcmp(new.from, user_name) == 0)
				{
					printf("%s[提醒]%s%s\n\n" ,ctime(&new.now_time),new.to,new.buf);
				}
				else
				{
					printf("%s[%s]%s\n\n",ctime(&new.now_time),new.from, new.buf);
					save_chat(new,new.from);
				}
				break;
			case 4://群聊
				if(new.from[0] == '\0')
				{
					printf("%s[提醒]%s%s\n\n" ,ctime(&new.now_time),new.grp.name,new.buf);
				}
				else
				{
					printf("%s[%s : %s]%s\n\n",ctime(&new.now_time), new.grp.name, new.from, new.buf);
				}
				break;
			case 5://好友管理
				if(new.flag == 0)
				{
					if(strcmp(new.from, user_name) == 0)
					{
						printf("%s[提醒]%s%s\n\n",ctime(&new.now_time), new.to, new.buf);
					}
					else
					{
						printf("%s[%s]请求添加您为好友,若同意请输入accept friend %s\n\n",ctime(&new.now_time),new.from, new.from);
					}
				}
				else if(new.flag == 1)
				{
					if(strcmp(new.from, user_name) == 0)
					{
						printf("%s[提醒]%s%s\n\n",ctime(&new.now_time), new.to, new.buf);
					}
					else
					{
						printf("%s[提醒]%s%s\n\n",ctime(&new.now_time),new.from, new.buf);
					}
				}
				else if(new.flag == 2)
				{
					if(strcmp(new.from, user_name) == 0)
					{
						printf("%s[提醒]%s%s\n\n",ctime(&new.now_time), new.to, new.buf);
					}
					else
					{
						printf("%s[提醒]%s%s\n\n",ctime(&new.now_time), new.from, new.buf);
					}
				}
				else if(new.flag == 3)
				{
					if(new.buf[0] == '\0')
					{
						printf("%s[提醒]您暂无好友，快去添加吧\n\n",ctime(&new.now_time));	
					}
					else
					{
						printf("%s\t\033[1;33m【在线】\033[0m\n",ctime(&new.now_time));
						printf("%s\n\n",new.buf);
					}
				}
				break;
			case 6://群组管理
			
				if(new.flag == 1)//创建
				{
					printf("%s[提醒]讨论组%s%s\n\n",ctime(&new.now_time),new.grp.name, new.buf);						
				}
				else if(new.flag == 2)//退出
				{
					printf("%s%s\n\n",ctime(&new.now_time), new.buf);
				}
				else if(new.flag == 3)//查看
				{
					printf("%s%s\n\n",ctime(&new.now_time), new.buf);
				}
				break;
			
			case 7://文件传输
				if(new.flag == 0)
				{
					printf("%s[%s]请求向您发送文件[%s],若同意请输入 accept %s %s\n\n",ctime(&new.now_time),new.from, new.file_send, new.from, new.file_send);
				}
				else if(new.flag == 1)
				{				

					char t[20];
					strcpy(t, new.from);
					strcpy(new.from, new.to);
					strcpy(new.to, t);
					
					new_t *temp = (new_t *)malloc(sizeof(new_t));
					memcpy(temp, &new, sizeof(new_t));
					temp->flag = 2;
					temp->conn_fd = conn_fd;
					pthread_create(&thread, NULL, (void*)send_file, temp);
				}
				else if(new.flag == 2)
				{

					file_fd = open(new.file_recv, O_WRONLY | O_CREAT | O_APPEND, 0666);
					int i=0;
					while(new.buf[i]!='\0')
					{
						write(file_fd, &new.buf[i++], 1);
					}
					//printf("[提醒]文件%s接收: \n",new.file_recv);
					close(file_fd);
				}

				break;
	
			default :
				break;
		}
	}

	return;
}

//创建套接字并连接服务端
int main()
{
	int conn_fd;
	new_t new;
    struct sockaddr_in serv_addr;
    pthread_t thread;


    
    memset(&serv_addr,0,sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT_NUM);
    inet_aton(IPADDR,&serv_addr.sin_addr);

	//创建套接字
    conn_fd = socket(AF_INET,SOCK_STREAM,0);
    if(conn_fd < 0)
    {
		perror("socket");
        return 0;
    }
    //连接服务器
    if(connect(conn_fd,(struct sockaddr*)&serv_addr,sizeof(struct sockaddr_in)) < 0)
    {
        perror("connect");
        return 0;
    }
	
	while(1)//注册登陆流程
	{		
		printf("<1>注册 <2>登陆 <0>退出\n");
		scanf("%d",&new.type);
		while(getchar()!='\n');
		switch(new.type)
		{
			case 1:
				printf("输入用户名：");
				scanf("%s",new.user.name);
				printf("输入密码：");
				getchar();
				system("stty -echo");
				system("stty -icanon");
				get_pass(new.user.pass);
				//scanf("%s",new.user.pass);
				system("stty echo");
				system("stty icanon");
				new.conn_fd = conn_fd;
				time(&new.now_time);
				send(conn_fd, &new, sizeof(new_t), 0);
				recv(conn_fd, &new, sizeof(new_t), 0);
				printf("[提醒]%s\n",new.buf);
				break;
			case 2:
				printf("输入用户名：");
				scanf("%s",new.user.name);
				time(&new.now_time);
				printf("输入密码：");
				getchar();
				system("stty -echo");
				system("stty -icanon");
				get_pass(new.user.pass);
				//scanf("%s",new.user.pass);
				system("stty echo");
				system("stty icanon");
				send(conn_fd, &new, sizeof(new_t), 0);				
				recv(conn_fd, &new, sizeof(new_t), 0);
				printf("[提醒]%s\n",new.buf);
				break;
			case 0:
				return 0;
			default:
				break;
		}
		
		//登陆成功后跳出循环
		if(strcmp(new.buf, "登陆成功") == 0)
		{				
			strcpy(user_name,new.user.name);
			break;
		}
	}	

	//创建线程用于接受服务端消息
	pthread_create(&thread, NULL, (void*)def, &conn_fd);

	char cmd[20],type[20],buf[500];

	while(1)
	{ //输入解析

		print();
		memset(&new, 0 ,sizeof(new_t));
		scanf("%s",cmd);
		if(strcmp(cmd, "quit") == 0)
		{
			printf("再见～\n");
			exit(1);
		}
		scanf("%s",type);

		scanf("%s",buf);
		
		new.conn_fd = conn_fd;
		time(&new.now_time);
		
		if(strcmp(cmd, "friend") == 0)//私聊
		{
			new.type = 3;
			strcpy(new.from, user_name);
			strcpy(new.to, type);
			strcpy(new.buf, buf);
			save_chat(new,new.to);
			send(conn_fd, &new, sizeof(new_t), 0);
		}
		else if(strcmp(cmd, "group") == 0)//群聊
		{
			new.type = 4;
			strcpy(new.from, user_name);
			strcpy(new.grp.name, type);
			strcpy(new.buf, buf);
			//save_chat(new,new.to);
			send(conn_fd, &new, sizeof(new_t), 0);
				
		}
		else if(strcmp(cmd, "add") == 0)
		{
			if(strcmp(type, "friend") == 0)//添加好友
			{
				new.type = 5;
				new.flag = 0;
				strcpy(new.from, user_name);
				strcpy(new.to, buf);
				send(conn_fd, &new, sizeof(new_t), 0);
			}
			else if(strcmp(type, "group") == 0)//创建群组
			{
				new.type = 6;
				new.flag = 1;
				strcpy(new.from, user_name);
				strcpy(new.grp.make, user_name);
				strcpy(new.grp.name, buf);
				printf("请输入讨论组成员人数：");
				do
				{
					scanf("%d",&new.grp.num);
				}while(new.grp.num<0);
				int i;
				for(i=0;i<new.grp.num;i++)
				{
					printf("输入成员%d：",i+1);
					scanf("%s",new.grp.people[i]);
				}						
				send(conn_fd, &new, sizeof(new_t), 0);		
			}
			else
			{
				printf("参数不合法\n");
			}				

		}
		else if(strcmp(cmd, "del") == 0)
		{
			if(strcmp(type, "friend") == 0)//删除好友
			{
				new.type = 5;
				new.flag = 2;
				strcpy(new.from, user_name);
				strcpy(new.to, buf);
				send(conn_fd, &new, sizeof(new_t), 0);
			}
			else if(strcmp(type, "group") == 0)//删除群组
			{
				new.type = 6;
				new.flag = 2;
				strcpy(new.from, user_name);
				strcpy(new.grp.name, buf);
				send(conn_fd, &new, sizeof(new_t), 0);
			}
			else
			{
				printf("参数不合法\n");
			}
		}
		else if(strcmp(cmd, "look") == 0)
		{
			if(strcmp(type, "friend") == 0)//查看好友
			{
				if(strcmp(buf, "all") != 0)
				{
					printf("参数不合法\n");
					continue;
				}
				new.type = 5;
				new.flag = 3;
				strcpy(new.from, user_name);
				strcpy(new.to, buf);
				send(conn_fd, &new, sizeof(new_t), 0);
			}
			else if(strcmp(type, "group") == 0)//查看群组
			{
				if(strcmp(buf, "me") != 0 && strcmp(buf, "all") != 0)
				{
					printf("参数不合法\n");
					continue;
				}
				new.type = 6;
				new.flag = 3;
				strcpy(new.from, user_name);
				strcpy(new.grp.name, buf);
				send(conn_fd, &new, sizeof(new_t), 0);			
			}
			else
			{
				printf("参数不合法\n");
			}
		}
		else if(strcmp(cmd, "accept") == 0)
		{
			if(strcmp(type, "friend") == 0)//接受好友请求
			{
				new.type = 5;
				new.flag = 1;
				strcpy(new.from, user_name);
				strcpy(new.to, buf);
				send(conn_fd, &new, sizeof(new_t), 0);
			}
			//if(strcmp(type, "file") == 0)//接受文件
			else
			{
				struct stat temp;
				printf("请输入接收后的文件名：");
				while(1)
				{
					scanf("%s",new.file_recv);	
					
					if(stat(new.file_recv, &temp) == 0)
					{
						printf("[提醒]%s已存在，请重新指定文件名\n",new.file_recv);
						continue;
					}
					else
					{
						break;
					}
				}
				new.type = 7;
				new.flag = 1;
				strcpy(new.from, user_name);
				strcpy(new.to, type);
				strcpy(new.file_send, buf);
				
				send(conn_fd, &new, sizeof(new_t), 0);
			}
			/*else
			{
				printf("参数不合法\n");
			}*/
		}
		else if(strcmp(cmd, "file") == 0)//发送文件
		{
			if(stat(buf, &new.file) == 0)
			{
				new.type = 7;
				new.flag = 0;
				strcpy(new.file_send, buf);
				strcpy(new.from, user_name);
				strcpy(new.to, type);
				//sprintf("%s");
				send(conn_fd, &new, sizeof(new_t), 0);
			}
			else
			{
				printf("该文件不存在\n");
			}
		}
		else
		{
			printf("命令不合法\n");
		}
	}
	
	return 0;	
}
