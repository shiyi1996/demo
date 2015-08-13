/*************************************************************************
	> File Name: common.c
	> Author: 师毅
	> Mail:  458241107@qq.com
	> Created Time: 2015年08月12日 星期三 13时51分24秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<fcntl.h>
#include"client.h"

void get_pass(char pass[])
{
	int i=0;
	while(1)
	{
		pass[i] = getchar();
		if(pass[i] == 127 && i>=0)
		{
			if(i==0)
			{
				continue;
			}
			putchar('\b');
			putchar(' ');
			putchar('\b');
			i--;
		}
		else if(pass[i] == '\n')
		{
			pass[i]='\0';
			puts("");
			break;
		}
		else
		{
			putchar('*');
			i++;
		}
	}
}

//文件传输
int send_file(new_t *new)
{
	int fd;

    fd = open(new->file_send, O_RDONLY | O_CREAT, 0666);

    while(read(fd, new->buf, 400))
    {

		if(send(new->conn_fd, new, sizeof(new_t), 0) < 0)
		{
			perror("send");
		}
		usleep(100000);
		memset(new->buf, 0, sizeof(new->buf));       
    }
    printf("[提醒]文件%s发送成功\n",new->file_send);
    close(fd);
	free(new);
	return 1;
}

//保存聊天记录
int save_chat(new_t new,char filename[])
{
	char file[20];
	char test[500];
	int fd;
	
	sprintf(file,"%s.txt",filename);

	fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0666);
	if(fd == -1)
	{
		perror("open");
	}
	
	sprintf(test, "%s[%s]%s\n\n",ctime(&new.now_time),new.from,new.buf);

	int i=0;
	
	while(test[i]!='\0')
	{
		write(fd, &test[i++], 1);
	}

    close(fd);
    return 1;
}

//输出提示信息
void print()
{
	printf("**********************************************************\n");
	printf("****      friend XXX NNN <---> 向好友XXX私信NNN       ****\n");
	printf("****       group XXX NNN <---> 向讨论组XXX群发NNN     ****\n");
	printf("****      add friend XXX <---> 添加好友XXX            ****\n");
	printf("****   accept friend XXX <---> 接受XXX的好友请求      ****\n");
	printf("****      del friend XXX <---> 删除好友XXX            ****\n");	
	printf("****       add group XXX <---> 创建讨论组XXX          ****\n");
	printf("****       del group XXX <---> 退出讨论组XXX          ****\n");
	printf("****     look friend all <---> 查看在线好友           ****\n");
	printf("****      look group all <---> 查看所有的讨论组       ****\n");
	printf("****        file XXX NNN <---> 向XXX发送文件NNN        ****\n");
	printf("****       look group me <---> 查看与自己有关的讨论组 ****\n");
	printf("****                quit <---> 退出                ****\n");	
	printf("**********************************************************\n");	
}

