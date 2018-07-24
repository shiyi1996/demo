/*===============================================================
*   Copyright (C) 2015 All rights reserved.
*   
*   文件名称：fcntl_access.c
*   创 建 者：师毅
*   创建日期：2015年07月19日
*   描    述：
*
*   更新日志：
*
================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
    int ret;
    int access_mode;
    int fd;

    if((fd = open("1.c", O_RDWR | O_CREAT | O_TRUNC, S_IRWXU))==-1)
    {
        printf("error : can't open file\n");
        exit(1);
    }

    if((ret = fcntl(fd, F_SETFL, O_APPEND))<0)
    {
        printf("error : setfilemode fail\n");
        exit(2);
    }
    
    if((ret = fcntl(fd, F_GETFL, 0)) <0)
    {
        printf("error : getfilemode fail\n");
        exit(3);
    }

    access_mode = ret & O_ACCMODE;
    printf("%o\n",access_mode);
    if(access_mode == O_RDWR)printf("wr\n");
    if(ret&O_APPEND)printf("append\n");
    return 0;
}
