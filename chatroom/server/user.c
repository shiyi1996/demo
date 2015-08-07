/*************************************************************************
	> File Name: user.c
	> Author: 师毅
	> Mail:  shiyi19960604@gmail.com
	> Created Time: 2015年08月03日 星期一 15时49分01秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include"user.h"

#define USER_FILE "user.txt"

//根据用户名查找id，若存在返回id，否则，返回0
int find_user_id(char name[], user_t *buf)
{
    int fd;

    fd = open(USER_FILE, O_RDONLY | O_CREAT, 0777);

    if(fd == -1)
    {
        perror("open");
        return 0;
    }

    while(read(fd, buf, sizeof(user_t)))
    {
        if(strcmp(buf->name, name) == 0)
        {
            close(fd);
            return buf->id;
        }
    }

    close(fd);

    return 0;
}

int find_user_name(int id, user_t *buf)
{
	int fd;

    fd = open(USER_FILE, O_RDONLY | O_CREAT, 0777);

    if(fd == -1)
    {
        perror("open");
        return 0;
    }

    while(read(fd, buf, sizeof(user_t)))
    {
        if(buf->id == id)
        {
            close(fd);
            return 1;
        }
    }

    close(fd);

    return 0;
}

int add_user(user_t *buf)
{
    int fd;

    buf->id = make_id();
    printf("qqqq : %d\n",buf->id);

    fd = open(USER_FILE, O_WRONLY | O_CREAT | O_APPEND, 0777);

    if(fd == -1)
    {
        perror("open");
        return 0;
    }
    
    if(write(fd,buf,sizeof(user_t)) == -1)
    {
        perror("write");
        return 0;
    }

    close(fd);
    return 1;
}

int ID=1;

int make_id()
{
    return ID++;
}
