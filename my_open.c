/*===============================================================
*   Copyright (C) 2015 All rights reserved.
*   
*   文件名称：my_open.c
*   创 建 者：师毅
*   创建日期：2015年07月18日
*   描    述：
*
*   更新日志：
*
================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main(int argc , char **argv)
{
    char *filename;
    int flags;
    int mode = 0;

    filename = argv[2];

    if(argc<3)//判断命令是否合法
    {
        printf("error : open <mode> <filename>\n");
        exit(0);
    }

    if(strlen(argv[1])>2 || 
        argv[1][1]!='+' && argv[1][1]!='\0' ||
       argv[1][0]!='w' && argv[1][0]!='r' && argv[1][0]!='a' )
    {
        printf("error : 文件打开方式不合法\n");
        exit(0);
    }

    umask(0);//设置允许当前进程创建文件或者目录最大可操作的权限
    //即 (~0)&mode;    

    if(argv[1][1]=='+')
    {
        switch(argv[1][0]) 
        {
            case 'w'://可读可写 文件不存在会重新创建 文件内容清空
                flags = O_RDWR | O_TRUNC | O_CREAT;
                mode = 6*8*8+6*8+6;
                break;
            case 'a'://可读可写 文件不存在会重新创建 文件内容保留 指针指向文件尾
                flags = O_RDWR | O_CREAT | O_APPEND;
                mode = 6*8*8+6*8+6;
                break;
            case 'r'://可读可写 文件不存在会出错 文件内容保留 指针指向文件头
                flags = O_RDWR;
                break;
        }
    }
    else
    {
        switch(argv[1][0]) 
        {
            case 'w':
                flags = O_WRONLY;
                break;
            case 'r':
                flags = O_RDONLY;
                break;
        }
    }
    
    if(mode == 0)
    {
        if(open(filename, flags) == -1)
	{
		printf("打开文件失败\n");
		exit(1);
	}
    }
    else
    {
        if(open(filename, flags, mode) == -1)
	{
		printf("打开文件失败\n");
		exit(1);
	}
    }

    return 0;
}
