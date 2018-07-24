/*===============================================================
*   Copyright (C) 2015 All rights reserved.
*   
*   文件名称：my_more.c
*   创 建 者：师毅
*   创建日期：2015年07月20日
*   描    述：
*
*   更新日志：
*
================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define PAGENUM 10 //定义一屏为10行

void do_more(int fd);
int see_more(  int sum, int now);

int main( int argc, char **argv )
{
    int fd;
    if( argc == 1 )//参数不合法则返回
    {
        return 0;
    }
    else
    {
        while( argc-->1 )
        {
            if( (fd = open(*++argv,O_RDONLY)) != -1 )
            {   
                do_more(fd);
                close(fd);
            }
            else
            {
                printf("Error: can't open file\n");
                exit(1);
            }
        }
    }
    return 0;
}

void do_more(int fd)
{
    char str[3];
    int page_num = 0;//保存每次需要跳转的行数
    int rel;
    int page_now = 0;//保存当前行数（初始为0）
    int page_sum;//保存总行数
 

    lseek(fd,0,SEEK_END);//移动文件指针至文件末
    page_sum = lseek(fd,0,SEEK_CUR);//计算与文件头偏移量，即文件总行数
    lseek(fd,0,SEEK_SET);//移动文件指针至文件头

    while( page_now<page_sum )
    {
        while(read(fd,str,1))//每次读一行
        {
            if(str[0] =='\n')
            {
                printf("\n");
                break;
            }
            printf("%c",str[0]);
        }
        page_now = lseek(fd, 0, SEEK_CUR);//实时更新已读行数
        if( page_num == PAGENUM )
        {
            rel = see_more( page_sum, page_now);//获取用户输入
            printf("\033[1A\033[K");//将光标上移一行，并清空该行内容，用以覆盖提示信息
            if( rel == 0 )
            {
                break;
            }
            else
            {
                page_num -= rel;
            }
        }
        page_num++;
    }
}

int see_more(int sum,int now)
{
    int rel;
    printf("\033[7m --更多-- enter翻行 backspace翻页 q退出  %.2f%% \033[0m",now*100.0/sum);//反显输出提示信息以及文件已读比例

    while(rel = getchar())
    {
        if(rel == 'q')
        {
            return 0;
		}
        if(rel == ' ')
        {
	    	return PAGENUM;
		}
        if(rel == '\n')
        {   
			return 1;
		}
    }
    
    return 0;
}
