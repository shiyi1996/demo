/*===============================================================
*   Copyright (C) 2015 All rights reserved.
*   
*   文件名称：my_rw.c
*   创 建 者：师毅
*   创建日期：2015年07月19日
*   描    述：
*
*   更新日志：
*
================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int my_read(int fd)
{
    int len,ret,i;
    char read_buf[100];

    lseek(fd,0,SEEK_END);
    len = lseek(fd,0,SEEK_CUR);
    lseek(fd,0,SEEK_SET);
    ret = read(fd,read_buf,len);
    read_buf[len] = '\0';
    puts(read_buf);
    
    return ret;
}



int main()
{
    int fd;
    char write_buf[]="I love the world";
    if((fd=open("1.c", O_RDWR | O_CREAT | O_TRUNC, S_IRWXU))==-1)
    {
        printf("error : can't open the 1.c\n");
        exit(1);
    }    
    else
    {
        printf("creat file success\n");
    }

    if(write(fd,write_buf,strlen(write_buf)) != strlen(write_buf))
    {
        printf("error : write\n");
        exit(2);
    }

    my_read(fd);
    
    close(fd);

    return 0;
}
