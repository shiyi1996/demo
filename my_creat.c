/*===============================================================
*   Copyright (C) 2015 All rights reserved.
*   
*   文件名称：my_creat.c
*   创 建 者：师毅
*   创建日期：2015年07月18日
*   描    述：
*
*   更新日志：
*
================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    char *filename;
    int fd;
    int mode=6*8*8+6*8+6;

    filename = argv[1];

    if(argc<2)
    {
        printf("error : creat <filename>\n");
        error(0);
    }

    if(fd = creat(filename, mode) == -1)
    {
        printf("打开或创建文件失败\n");
        error(1);
    }

    close(fd);

    return 0;
}
